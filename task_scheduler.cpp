#include "task_scheduler.hpp"

uintptr_t task_scheduler::get_address() {
    /*
    // Scans through the Roblox memory region (from base address to base address + image size)
    uintptr_t task_scheduler = driver->find_pattern(driver->base, "11 11 11 11 11 11 71 3f 00 00 00 00 00 00", 0x20); // base address, signature, offset
    return task_scheduler;
    */

    return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_offset);
}

uintptr_t task_scheduler::get_array_size() {
    return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_offset + 0x8);
}

std::string task_scheduler::get_job_name(uintptr_t job) {
    uintptr_t name_ptr = job + offsets::job_name;
    const int string_length = driver->read<int>(name_ptr + 0x18);

    if (string_length >= 16) // if string goes over 16 characters, it will be stored using a pointer
        name_ptr = driver->read<uintptr_t>(name_ptr);

    std::vector<char> buffer(256);
    driver->read_physical(name_ptr, buffer.data(), buffer.size());

    return std::string(buffer.data());
}

std::vector<uintptr_t> task_scheduler::get_array() {
    std::vector<uintptr_t> jobs;

    uintptr_t scheduler_base = get_address();
    uintptr_t array_end = get_array_size();

    for (uintptr_t i = 0; (scheduler_base + i) < array_end; i += 0x10) {
        uintptr_t job = driver->read<uintptr_t>(scheduler_base + i);

        if (get_job_name(job).empty())
            continue;

        if (job) {
            jobs.push_back(job);
        }
    }

    return jobs;
}

std::vector<uintptr_t> task_scheduler::get_jobs(const std::string& name) {
    std::vector<uintptr_t> result;

    for (const auto& job : get_array()) {
        if (get_job_name(job) == name)
            result.push_back(job);
    }

    return result;
}

uintptr_t task_scheduler::get_job(const std::string& name) {
    for (const auto& job : get_array()) {
        if (get_job_name(job) == name)
            return job;
    }

    return 0x0;
}

void task_scheduler::print_jobs() {
    for (const auto& job : get_array()) {
        printf("Task Scheduler: Found job at <0x%llx - %s>\n", job, get_job_name(job).c_str());
    }
}

uintptr_t task_scheduler::get_renderview() {
    uintptr_t render_job = get_job("RenderJob(EarlyRendering;Ugc)");

    return driver->read<uintptr_t>(render_job + offsets::renderjob::renderview_ptr);
}

uintptr_t task_scheduler::get_datamodel() {
    uintptr_t render_job = get_job("RenderJob(EarlyRendering;Ugc)");

    return driver->read<uintptr_t>(render_job + offsets::renderjob::datamodel_ptr) + offsets::renderjob::datamodel_offset;
}

uintptr_t task_scheduler::get_visualengine() {
    uintptr_t render_job = get_job("RenderJob(EarlyRendering;Ugc)");

    uintptr_t renderview = driver->read<uintptr_t>(render_job + offsets::renderjob::renderview_ptr);
    return driver->read<uintptr_t>(renderview + offsets::renderjob::visualengine_ptr);
}

bool task_scheduler::is_loaded() {
    return get_array().size() != 0;
}
