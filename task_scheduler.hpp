#pragma once
#include "../includes.h"

namespace task_scheduler {
	namespace offsets {
		// (?? ?? 00 00 11 11 11 11 11 11 71 3f 00 00 00 00) + 0x24
		static uintptr_t task_scheduler_ptr = 0x5CA7018; // version-dd2acaf7460f42ee
		static uintptr_t job_name = 0x90;

		namespace renderjob {
			static uintptr_t renderview_ptr = 0x218;
		}
	}

	uintptr_t	get_scheduler();
	std::string	get_job_name(uintptr_t job);

	std::vector<uintptr_t>	active_jobs();
	std::vector<uintptr_t>	get_jobs(const std::string& name);
	uintptr_t		get_job(const std::string& name);
	void			print_jobs();

	uintptr_t	get_renderview();
	bool		is_loaded();
}
