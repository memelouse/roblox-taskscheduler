#pragma once
#include "../includes.h"

namespace task_scheduler {
	namespace offsets {
		static uintptr_t task_scheduler_ptr = 0x5C71CC8; // version-ecc9c250281b4c14

		static uintptr_t job_name = 0x90;
		static uintptr_t size = 0x3C8; // getting the size manually lol

		namespace renderjob {
			static uintptr_t renderview_ptr = 0x218;
		}
	}

	uintptr_t get_scheduler();

	std::vector<uintptr_t> 	active_jobs();
	std::vector<uintptr_t> 	get_jobs(const std::string& name);
	uintptr_t 		get_job(const std::string& name);
	void 			print_jobs();

	uintptr_t 	get_renderview();
	bool 		is_loaded();
}
