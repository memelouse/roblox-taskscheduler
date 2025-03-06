#include "task_scheduler.hpp"
#include "../driver/communications.hpp"

uintptr_t task_scheduler::get_scheduler() {
    /*
    uintptr task_scheduler_ptr = driver->pattern_scan(_("11 11 11 11 11 11 71 3f 00 00 00 00 00 00"), 0x20);
    return driver->read<uintptr_t>(task_scheduler_ptr);
    */
    
    return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_ptr);
}

std::string task_scheduler::get_job_name(uintptr_t job) {
    // If the string exceeds 16 characters, it will be stored as a pointer.
    
    /*
    * here is an example
    uintptr_t to_read = job + offsets.job_name;
    const int length = driver->read<int>(to_read + 0x18); // string size

    if (length >= 16) {
        uintptr_t ptr = driver->read<uintptr_t>(to_read);
        to_read = ptr;
    }

    std::vector<char> buffer(256);
    driver->read_phys_memory(reinterpret_cast<PVOID>(to_read), buffer.data(), buffer.size());

    return std::string(buffer.data());
    */
    
    return RBX::get_string(job + offsets::job_name);
}

std::vector<uintptr_t> task_scheduler::active_jobs() {
    std::vector<uintptr_t> jobs;
    uintptr_t task_scheduler = get_scheduler();

    uint64_t scheduler_metadata = driver->read<uint64_t>(task_scheduler - sizeof(uint64_t));
    int p_total_jobs = (scheduler_metadata >> 54) & 0x3F; // Not sure about this

    // Scan through possible job entries
    for (uintptr_t job = driver->read<uintptr_t>(task_scheduler), job_index = 0;
        job_index <= p_total_jobs && job != 0x0;
        job_index += 1, job = driver->read<uintptr_t>(task_scheduler + (job_index * sizeof(std::shared_ptr<void*>)))) {

        if (get_job_name(job).empty())
            continue; // Skip jobs without a name

        jobs.push_back(job);
    }

    return jobs;
}

std::vector<uintptr_t> task_scheduler::get_jobs(const std::string& name) {
    std::vector<uintptr_t> result;

    // Filter out jobs by matching the provided name
    for (const auto& job : active_jobs()) {
        if (get_job_name(job) == name)
            result.push_back(job);
    }

    return result;
}

uintptr_t task_scheduler::get_job(const std::string& name) {
    for (const auto& job : active_jobs()) {
        if (get_job_name(job) == name)
            return job;
    }

    return 0x0;
}

void task_scheduler::print_jobs() {
    for (const auto& job : active_jobs()) {
        printf("Task Scheduler: Found job at <0x%llx - %s>\n", job, get_job_name(job).c_str());
    }
}

uintptr_t task_scheduler::get_renderview() {
    const uintptr_t render_job = get_job("RenderJob");

    return driver->read<uintptr_t>(render_job + offsets::job_renderview_ptr);
}

uintptr_t task_scheduler::get_datamodel() {
    const uintptr_t render_job = get_job("RenderJob");

    return driver->read<uintptr_t>(render_job + offsets::datamodel_ptr) + offsets::datamodel_offset;
}

uintptr_t task_scheduler::get_visualengine() {
    const uintptr_t render_job = get_job("RenderJob");

    uintptr_t renderview = driver->read<uintptr_t>(render_job + offsets::job_renderview_ptr);
    return driver->read<uintptr_t>(renderview + offsets::visualengine_ptr);
}

bool task_scheduler::is_loaded() {
    // Check if there are no active jobs (indicating the scheduler isn't loaded/not found)
    return active_jobs().size() != 0;
}
