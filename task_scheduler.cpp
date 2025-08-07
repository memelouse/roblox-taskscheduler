#include "task_scheduler.hpp"

std::string job_t::get_name() {
    uintptr_t name_ptr = this->address + offsets::JOB_NAME;
    int string_length = driver->read<int>(name_ptr + 0x18);

    if (string_length >= 16)
        name_ptr = driver->read<uintptr_t>(name_ptr);

    std::vector<char> buffer(256);
    driver->read_physical(name_ptr, buffer.data(), buffer.size());

    return std::string(buffer.data());
}


uintptr_t task_scheduler_t::get_job_array_start() {
    return driver->read<uintptr_t>(driver->base + offsets::TASK_SCHEDULER);
}

uintptr_t task_scheduler_t::get_job_array_end() {
    return driver->read<uintptr_t>(driver->base + offsets::TASK_SCHEDULER + sizeof(uintptr_t));
}

std::vector<job_t> task_scheduler_t::get_all_jobs() {
    std::vector<job_t> jobs;

    uintptr_t start = this->get_job_array_start();
    uintptr_t end = this->get_job_array_end();

    for (uintptr_t addr = start; addr < end; addr += 0x10) {
        job_t job = driver->read<job_t>(addr);

        if (job.address && !job.get_name().empty()) {
            jobs.push_back(job);
        }
    }

    return jobs;
}

uintptr_t task_scheduler_t::get_job(const std::string& name) {
    for (job_t job : this->get_all_jobs()) {
        if (job.get_name().find(name) != std::string::npos)
            return job.address;
    }

    return 0;
}

void task_scheduler_t::print_jobs() {
    for (job_t job : this->get_all_jobs()) {
        printf("Job Name: %s, Address: 0x%llx\n", job.get_name().c_str(), job.address);
    }
}

uintptr_t task_scheduler_t::get_renderview() {
    uintptr_t render_job = this->get_job("RenderJob(EarlyRendering");
    if (!render_job)
        return 0;

    return driver->read<uintptr_t>(render_job + offsets::RENDER_VIEW_PTR);
}

uintptr_t task_scheduler_t::get_datamodel() {
    uintptr_t render_job = this->get_job("RenderJob(EarlyRendering");
    if (!render_job)
        return 0;

    uintptr_t datamodel_ptr = driver->read<uintptr_t>(render_job + offsets::DATA_MODEL_PTR);
    return datamodel_ptr + offsets::DATA_MODEL;
}

uintptr_t task_scheduler_t::get_visualengine() {
    uintptr_t render_view = this->get_renderview();
    if (!render_view)
        return 0;

    return driver->read<uintptr_t>(render_view + offsets::VISUAL_ENGINE_PTR);
}