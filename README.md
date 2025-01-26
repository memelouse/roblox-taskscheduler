# roblox-taskscheduler
This is a simple external method on how to get the renderview with the task scheduler.

The last offset update was on ```version-ecc9c250281b4c14```


# example
```c++
if (task_scheduler::is_loaded()) {
  task_scheduler::print_jobs();
  
  uintptr_t renderview = task_scheduler::get_renderview();
  printf("renderview = 0x%llx\n", renderview);
}
```
