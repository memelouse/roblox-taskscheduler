#include "task_scheduler.hpp"
#include "../driver/communications.hpp"

uintptr_t task_scheduler::get_scheduler() {
    return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_ptr);
}

std::string task_scheduler::get_job_name(uintptr_t job) {
    // If the string exceeds 16 characters, it will be stored as a pointer.
    return RBX::get_string(job + offsets::job_name);
}

std::vector<uintptr_t> task_scheduler::active_jobs() {
    std::vector<uintptr_t> jobs;
    uintptr_t task_scheduler = get_scheduler();

    // Scan through possible job entries (size of each pointer is the size of std::shared_ptr<void*> = 0x8)
    for (uintptr_t i = 0x0; i <= 0x500; i += sizeof(std::shared_ptr<void*>)) {
        uintptr_t job = driver->read<uintptr_t>(task_scheduler + i);

        if (job == 0x0)
            break; // Stop when no more jobs are found

        if (get_job_name(job).empty())
            continue; // Skip jobs without a name

        if (driver->is_valid(job))
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
        printf("[DEBUG] Task Scheduler: Found job at <0x%llx - %s>\n", job, get_job_name(job).c_str());
    }
}

uintptr_t task_scheduler::get_renderview() {
    const uintptr_t render_job = get_job("RenderJob");

    return driver->read<uintptr_t>(render_job + offsets::job_renderview_ptr);
}

bool task_scheduler::is_loaded() {
    // Check if there are no active jobs (indicating the scheduler isn't loaded/not found)
    return active_jobs().size() != 0;
}
