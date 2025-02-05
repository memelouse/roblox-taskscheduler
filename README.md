# roblox-taskscheduler
This is a simple external method for finding renderview using the task scheduler.

The project will be updated with each new Roblox version until I decide to discontinue it.

The latest offset update was in ```version-b7a853f1584049b4```

If you encounter any issues, feel free to contact me on Discord.
# example
```c++
if (task_scheduler::is_loaded()) {
  task_scheduler::print_jobs();
  
  uintptr_t renderview = task_scheduler::get_renderview();
  printf("renderview = 0x%llx\n", renderview);
}
```
