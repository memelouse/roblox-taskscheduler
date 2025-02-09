# roblox-taskscheduler
This is a simple external method for finding (RenderView, DataModel, VisualEngine) using the task scheduler.

The project will be updated with each new Roblox version until I decide to discontinue it.

The latest offset update was in ```version-f9977b146957492c```

If you encounter any issues, feel free to contact me on Discord.
# example
```c++
if (task_scheduler::is_loaded()) {
  task_scheduler::print_jobs();
  
  uintptr_t renderview = task_scheduler::get_renderview();
  printf("renderview = 0x%llx\n", renderview);

  uintptr_t datamodel = task_scheduler::get_datamodel();
  printf("datamodel = 0x%llx\n", datamodel);

  uintptr_t visualengine = task_scheduler::get_visualengine();
  printf("visualengine = 0x%llx\n", visualengine);
}
```
