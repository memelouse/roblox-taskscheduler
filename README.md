this is a simple way on how to get the renderview with the task scheduler.

the offsets was updated on ```version-ecc9c250281b4c14```


# example
```
if (task_scheduler::is_loaded()) {
  task_scheduler::print_jobs();
  
  uintptr_t renderview = task_scheduler::get_renderview();
  printf("renderview = 0x%llx\n", renderview);
}
```
