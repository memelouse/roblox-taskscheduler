#include "task_scheduler.hpp"

uintptr_t task_scheduler::get_address() {
	/*
    uintptr_t task_scheduler = driver->find_pattern(driver->base, "11 11 11 11 11 11 71 3f 00 00 00 00 00 00", 0x20);
    return task_scheduler;
    */

    return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_offset);
}

uintptr_t task_scheduler::get_array_size() {
	return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_offset + 0x8);
}

std::string task_scheduler::get_job_name(uintptr_t job) {
    uintptr_t to_read = job + offsets::job_name;
    const int length = driver->read<int>(to_read + 0x18); // string size

    if (length >= 16) {
        uintptr_t ptr = driver->read<uintptr_t>(to_read);
        to_read = ptr;
    }

    std::vector<char> buffer(256);
    driver->read_physical(to_read, buffer.data(), buffer.size());

    return std::string(buffer.data());
}

std::vector<uintptr_t> task_scheduler::get_array() {
    std::vector<uintptr_t> jobs;

    const uintptr_t task_scheduler = get_address();
	const uintptr_t size_ptr = get_array_size();

    for (uintptr_t job = driver->read<uintptr_t>(task_scheduler), i = 0;
        task_scheduler + i < size_ptr;
        i += 0x8, job = driver->read<uintptr_t>(task_scheduler + i)) {

        if (get_job_name(job).empty())
            continue; // Skip jobs without a name

        if (job)
            jobs.push_back(job);
    }

    return jobs;
}

std::vector<uintptr_t> task_scheduler::get_jobs(const std::string& name) {
    std::vector<uintptr_t> result;

    // Filter out jobs by matching the provided name
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
    const uintptr_t render_job = get_job("RenderJob");

    return driver->read<uintptr_t>(render_job + offsets::renderjob::renderview_ptr);
}

uintptr_t task_scheduler::get_datamodel() {
    const uintptr_t render_job = get_job("RenderJob");

    return driver->read<uintptr_t>(render_job + offsets::renderjob::datamodel_ptr) + offsets::renderjob::datamodel_offset;
}

uintptr_t task_scheduler::get_visualengine() {
    const uintptr_t render_job = get_job("RenderJob");

    uintptr_t renderview = driver->read<uintptr_t>(render_job + offsets::renderjob::renderview_ptr);
    return driver->read<uintptr_t>(renderview + offsets::renderjob::visualengine_ptr);
}

bool task_scheduler::is_loaded() {
    // Check if there are no active jobs (indicating the scheduler isn't loaded/not found)
    return active_jobs().size() != 0;
}
