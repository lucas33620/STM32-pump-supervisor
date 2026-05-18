# Drivers_ext

Driver layer containing component-oriented software modules.

This layer provides software interfaces for hardware-related components:
- temperature sensor driver
- pump control driver
- LED driver
- CAN message codec or similar protocol helpers

Responsibilities:
- implement component-level behavior
- expose clear APIs to upper layers
- isolate device-specific details from App and Services

Rules:
- may use BSP abstractions
- must not contain application decision logic
- should remain focused on one device or one technical module