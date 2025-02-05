# roblox-taskscheduler
This is a simple external method for finding renderview using the task scheduler.

The project will be updated with each new Roblox version until I decide to discontinue it.

The latest offset update was in ```version-dd2acaf7460f42ee```

If you encounter any issues, feel free to contact me on Discord.
# example
```c++
if (task_scheduler::is_loaded()) {
  task_scheduler::print_jobs();
  
  uintptr_t renderview = task_scheduler::get_renderview();
  printf("renderview = 0x%llx\n", renderview);
}
```
