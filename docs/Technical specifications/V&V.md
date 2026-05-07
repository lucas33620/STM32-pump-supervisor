# Verification & Validation

| Requirement ID         | Verification Method                 | Success Criteria                                                                                              | Owner     |
| ---------------------- | ----------------------------------- | ------------------------------------------------------------------------------------------------------------- | --------- |
| FR-001, FR-002         | Bench Test, CAN Trace Analysis      | Startup completes correctly and reset cause is observable through diagnostics.                                | Developer |
| FR-003, FR-004, FR-005 | Bench Test, Fault Injection         | Temperature acquisition is periodic and sensor-related faults are detected within specified timing.           | Developer |
| FR-006, FR-007         | Bench Test                          | Actuator control follows threshold and hysteresis rules.                                                      | Developer |
| FR-008, FR-009         | CAN Trace Analysis                  | Heartbeat and status messages are transmitted at the defined period with valid content.                       | Developer |
| FR-010, FR-011         | Bench Test, CAN Trace Analysis      | Valid CAN diagnostic commands are processed correctly and invalid commands do not alter safe system behavior. | Developer |
| FR-012, FR-013         | Fault Injection, CAN Trace Analysis | Faults are reported correctly and critical faults trigger DEGRADED mode.                                      | Developer |
| FR-014, FR-015         | Fault Injection, Bench Test         | Task supervision and watchdog gating behave according to policy.                                              | Developer |
| FR-016                 | Bench Test                          | Degraded-mode observability and end-to-end DiagPack CAN scenarios are demonstrated.                           | Developer |
| NFR-001 to NFR-008     | Inspection, Bench Test, Unit Test   | Architecture, observability, reproducibility, documentation, and testability objectives are met.              | Developer |

Possible verification methods:

- Inspection
- Bench Test
- CAN Trace Analysis
- Fault Injection
- Unit Test
