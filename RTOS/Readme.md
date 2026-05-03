# RTOS

RTOS integration layer.

This layer contains the task-level orchestration of the subsystem:
- task creation
- task entry points
- scheduler-related hooks
- RTOS integration support files

Responsibilities:
- execute periodic and asynchronous software activities
- dispatch work to App and Services layers
- host FreeRTOS-related glue code

Rules:
- must not become a container for business logic
- task files should orchestrate behavior, not implement full application rules
- application logic should remain in App