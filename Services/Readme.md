# Services

Service layer containing transversal system managers.

This layer provides reusable internal services used by the application:
- diagnostic management
- fault management
- watchdog supervision
- other cross-cutting runtime services

Responsibilities:
- centralize subsystem support mechanisms
- provide observability and fault handling features
- support system supervision and runtime health logic

Rules:
- should not contain low-level HAL access directly unless explicitly wrapped
- should not contain product-specific control logic
- may depend on Drivers, BSP, and Common