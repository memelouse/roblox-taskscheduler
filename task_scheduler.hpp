#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace task_scheduler {
	namespace offsets {
		// "11 11 11 11 11 11 71 3f 00 00 00 00 00 00" + 0x20
		static uintptr_t task_scheduler_offset = 0x627DAE0;
		static uintptr_t job_name = 0x90;

		namespace renderjob {
			static uintptr_t renderview_ptr = 0x218;
			static uintptr_t visualengine_ptr = 0x10;

			static uintptr_t datamodel_ptr = 0xB0;
			static uintptr_t datamodel_offset = 0x1A0;
		}
	}

	uintptr_t	get_address();
	uintptr_t 	get_array_size();
	std::string	get_job_name(uintptr_t job);

	std::vector<uintptr_t>	get_array();
	std::vector<uintptr_t>	get_jobs(const std::string& name);
	uintptr_t				get_job(const std::string& name);
	void					print_jobs();

	uintptr_t	get_renderview();
	uintptr_t	get_datamodel();
	uintptr_t	get_visualengine();

	bool		is_loaded();
}
