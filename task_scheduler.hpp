#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace task_scheduler {
	namespace offsets {
		// (?? ?? ?? ?? ?? ?? 00 00 11 11 11 11 11 11 71 3f 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00) + 0x24
		static uintptr_t task_scheduler_ptr = 0x5CDCEA8;
		static uintptr_t job_name = 0x90;

		namespace renderjob {
			static uintptr_t renderview_ptr = 0x218;

			static uintptr_t datamodel_ptr = 0xB0;
			static uintptr_t datamodel_offset = 0x190;
		}
	}

	uintptr_t	get_scheduler();
	std::string	get_job_name(uintptr_t job);

	std::vector<uintptr_t>	active_jobs();
	std::vector<uintptr_t>	get_jobs(const std::string& name);
	uintptr_t		get_job(const std::string& name);
	void			print_jobs();

	uintptr_t	get_renderview();
	uintptr_t	get_datamodel();
	bool		is_loaded();
}
