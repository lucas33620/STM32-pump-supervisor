# System Overview

## Purpose of This Document

This document defines the technical scope, system context, constraints, functional requirements, non-functional requirements, interfaces, fault handling strategy, and verification approach for the STM32 Pump Supervisor.

The objective is to specify a realistic embedded system based on STM32 and FreeRTOS, designed to supervise a simple thermal circulation module, improve observability, detect abnormal behhaviors and support diagnostic and validation through CAN communications.

## General Description

The STM32 Pump Supervisor is a embedded subsystem designed to supervise a simple thermal circulation module. Its main function is to acquire temperature data from a real sensor, control a pump or equivalent actuator according to simple threshold rules and provide diagnostic visibility trough CAN communication.

## Functional Block Architecture

| Block                    | Description                                                                                                                                                                                                                                                                 |
| ------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Sensors                  | - A real temperature sensor provides the primary physical input of the system (MCP 9808).<br/>- The sensor is used to monitor the thermal state of the supervised module.<br/>- Sensor faults such as timeout or invalid values shall be detectable.                        |
| MCU                      | - The controller is a STM32F439ZITx (nucleo)<br/>-  It is responsible for sensor acquisition, actuator control, diagnostic handling, and system monitoring.                                                                                                                 |
| Power supply             | - Power source adapted to the STM32 board and actuator.                                                                                                                                                                                                                     |
| Communication Interfaces | - The main communication interface is CAN bus<br/>- CAN is used to transmit heartbeat, status, fault, and diagnostic messages.<br/>- An optional UART interface may be used for local debug.                                                                                |
| Actuators                | - The main actuator is a mini pump or a simple equivalent load such as a small fan.<br/>- The actuator is controlled by the MCU according to temperature supervision rules and fault policy.<br/>- In degraded or fault conditions, the actuator may be limited or stopped. |
| User Interfaces          | - The main operational visibility is provided through diagnostic communication                                                                                                                                                                                              |

## Context

### Problem statement

In embedded systems using pump or similare field, failures are not always visible or easy to dianose. A system may appear functional during basic opration while still suffering from :

- intermittent sensor faults

- timing drifts or tasks overruns

- poor observability of internal state

- lack of clear fault reporting

- difficult field diagnosis after unexpected beahvior or reset

The STM32 Pump Supervisor is intended to address these issues by providing supervision, fault detection, degraded operation, and diagnostic communication over CAN.

### Use-case

- As a validation engineer, I want the system to detect abnormal operating conditions so that unexpected behavior can be identified early during bench testing.
- As a diagnostic engineer, I want the system to expose its internal state, health information, and active faults through CAN so that I can understand its behavior without relying on internal firmware access.
- As a test engineer, I want to inject diagnostic commands and fault scenarios through CAN so that I can reproduce failure cases and verify the system reaction.
- As a bench integration engineer, I want the system to support bench testing with DiagPack CAN so that I can monitor messages, replay scenarios, and validate diagnostic behavior efficiently.

### Operating Context

The system runs on an STM32 microcontroller and uses FreeRTOS for task scheduling. It is intended for bench-level validation and embedded firmware demonstration, with at least one real primary component:

- a temperature sensor
- a mini pump or equivalent simple actuator

## Scope

### In Scope

The following items are included in the project scope:

- STM32-based embedded firmware
- FreeRTOS-based task architecture
- acquisition of a real temperature sensor
- control of a simple actuator (pump or equivalent)
- CAN communication for status, metrics, and diagnostic commands
- basic thermal supervision logic using thresholds and hysteresis
- fault detection and classification
- degraded mode behavior
- watchdog supervision strategy
- reset cause reporting
- compatibility with DiagPack CAN for monitoring, replay, and fault injection
- test scenarios focused on realistic diagnostic and robustness use cases

### Out of Scope

The following items are explicitly out of scope:

- CANopen, J1939 or other higher-level CAN protocols

- graphical user interface on the target (IHM)

- cloud connectivity

- wireless communication

- multiple pumps or multi-zone thermal management

- production-grade hardware safety certification

- bootloader and firmware update mechanism

- 

BAQUEY Lucas - SYLORIA
