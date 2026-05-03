# App

Application layer containing the subsystem business logic.

This layer defines the expected functional behavior of the Pump Supervisor:
- thermal supervision
- actuator decision logic
- operating modes
- high-level system behavior

Responsibilities:
- apply application rules and thresholds
- decide actuator commands from validated inputs
- manage transitions between operating modes
- expose a coherent subsystem state

Rules:
- must not directly access HAL APIs
- should not contain low-level peripheral handling
- should rely on Services, Drivers, and Common abstractions