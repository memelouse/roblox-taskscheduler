# Roblox Task Scheduler

A lightweight, external method to get (`RenderView`, `DataModel`, `VisualEngine`) using the task scheduler.

## Overview

It is actively maintained and updated with each new Roblox version—until I decide to discontinue support.

## Latest Update
- **Version**: `version-e00a4ca39fb04359`
- **Last Offset Update**: 12/03/2025

## Example
```cpp
#include "task_scheduler.hpp"

int main() {
    auto task_scheduler = std::make_unique<task_scheduler_t>();

    if (task_scheduler->is_loaded()) {
        // Print all active jobs within the task scheduler.
        task_scheduler->print_jobs();

        // Get and display the RenderView address.
        uintptr_t renderview = task_scheduler->get_renderview();
        printf("RenderView: 0x%llx\n", renderview);

        // Get and display the DataModel address.
        uintptr_t datamodel = task_scheduler->get_datamodel();
        printf("DataModel: 0x%llx\n", datamodel);

        // Get and display the VisualEngine address.
        uintptr_t visualengine = task_scheduler->get_visualengine();
        printf("VisualEngine: 0x%llx\n", visualengine);
    }

    return 0;
}
