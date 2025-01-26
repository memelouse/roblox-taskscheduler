#include "task_scheduler.hpp"
#include "../driver/communications.hpp"

uintptr_t task_scheduler::get_scheduler() {
    return driver->read<uintptr_t>(driver->base + offsets::task_scheduler_ptr);
}

std::vector<uintptr_t> task_scheduler::active_jobs() {
    std::vector<uintptr_t> jobs;
    uintptr_t task_scheduler = get_scheduler();

    for (uintptr_t i = 0x0; i <= offsets::size; i += sizeof(std::shared_ptr<void*>)) {
        uintptr_t ptr = driver->read<uintptr_t>(task_scheduler + i);

        if (RBX::get_string(ptr + offsets::job_name).empty())
            continue;

        if (driver->is_valid(ptr)) {
            jobs.push_back(ptr);
        }
    }

    return jobs;
}

uintptr_t task_scheduler::get_job(const std::string& name) {
    for (const auto& job : active_jobs()) {
        if (RBX::get_string(job + offsets::job_name) == name) {
            return job;
        }
    }

    return 0;
}

void task_scheduler::print_jobs() {
    for (const auto& job : active_jobs()) {
        printf(_("[DEBUG] Task Scheduler: <%s> 0x%llx\n"), RBX::get_string(job + offsets::job_name).c_str(), job);
    }
}

uintptr_t task_scheduler::get_renderview() {
    uintptr_t render_job = get_job(_("RenderJob"));

    return driver->read<uintptr_t>(render_job + offsets::renderjob::renderview_ptr);
}

std::vector<uintptr_t> task_scheduler::get_jobs(const std::string& name) {
    std::vector<uintptr_t> result;

    for (const auto& job : active_jobs()) {
        if (RBX::get_string(job + offsets::job_name) == name) {
            result.push_back(job);
        }
    }

    return result;
}

bool task_scheduler::is_loaded() {
    return get_jobs(_("WaitingHybridScriptsJob")).size() == 2;
}
