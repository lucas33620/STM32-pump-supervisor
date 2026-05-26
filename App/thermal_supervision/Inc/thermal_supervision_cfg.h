/**
 * @file    thermal_supervision_cfg.h
 * @brief   Configuration header for thermal supervision of MCP9808 temperature sensor on STM32 microcontrollers.
 * @author  Syloria, Lucas Baquey
 * * @copyright MIT License
 * * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef THERMAL_SUPERVISION_CFG_H
#define THERMAL_SUPERVISION_CFG_H

/**
 *  @brief Only thermal supervision configuration may be changed here.
 */

/** @section Defines */
/**< Temperature minimum threshold in degrees Celsius x 10 */
#define THERMAL_SUPERVISION_TEMP_MIN_THRESHOLD_X10      (-200) /* Temperature minimum in degrees Celsius x 10 */

/**< Temperature maximum threshold in degrees Celsius x 10 */
#define THERMAL_SUPERVISION_TEMP_MAX_THRESHOLD_X10      (1000) /* Temperature maximum in degrees Celsius x 10 */

/**< Overtemperature fault threshold in degrees Celsius x 10 */
#define THERMAL_SUPERVISION_OVERTEMP_FAULT_THRESHOLD_X10 (700) /* Overtemperature fault threshold in degrees Celsius x 10 */

/**< Temperature below which overtemperature recovery becomes possible */
#define THERMAL_SUPERVISION_OVERTEMP_RECOVERY_THRESHOLD_X10 (600) /* Temperature below which overtemperature recovery becomes possible */

#endif /* THERMAL_SUPERVISION_CFG_H */
