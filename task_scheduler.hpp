#pragma once

#include <vector>
#include <string>
#include <cstdint>

enum offsets : uintptr_t {
	TASK_SCHEDULER = 0x80AB3A0, // "11 11 11 11 11 11 91 3f 00 00 00 00 00 00" + 0x20 (0x3F71111111111111)

	JOB_NAME = 0x138,

	RENDER_VIEW_PTR = 0x218,
	VISUAL_ENGINE_PTR = 0x10,

	DATA_MODEL_PTR = 0x208,
	DATA_MODEL = 0x1B8
};

class job_t final {
public:
	job_t() {};
	job_t(uintptr_t address) : address(address) {};

	std::string get_name();

	uintptr_t address = 0;
};

class task_scheduler_t final {
public:
	task_scheduler_t() {};

	uintptr_t get_renderview();
	uintptr_t get_datamodel();
	uintptr_t get_visualengine();

	bool is_loaded() { return this->get_all_jobs().size() > 0; }
	void print_jobs();

private:
	uintptr_t get_job_array_start();
	uintptr_t get_job_array_end();

	std::vector<job_t> get_all_jobs();

	uintptr_t get_job(const std::string& name);
};
