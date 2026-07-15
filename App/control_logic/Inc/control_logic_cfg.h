/**
 * @file    control_logic_cfg.h
 * @brief   Configuration header for control logic of pumping system on STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef CONTROL_LOGIC_CFG_H
#define CONTROL_LOGIC_CFG_H

/**
 *  @brief Only control logic configuration may be changed here.
 */

/** @section Defines */

/**< Temperature ON threshold in degrees Celsius x 10 */
#define CONTROL_LOGIC_TEMP_ON_THRESHOLD_X10      (400) /* Temperature ON threshold in degrees Celsius x 10 */

/**< Temperature OFF threshold in degrees Celsius x 10 */
#define CONTROL_LOGIC_TEMP_OFF_THRESHOLD_X10      (350) /* Temperature OFF threshold in degrees Celsius x 10 */

#endif /* CONTROL_LOGIC_CFG_H */
