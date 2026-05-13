/**
 * @file    mcp9808_cfg.h
 * @brief   Project configuration constants (timing, retries, thresholds).
 * @author  Syloria, Lucas Baquey
 *
 * @copyright MIT License
 * Summary: Permission is granted, free of charge, to any person obtaining a copy
 * of this software to deal in the Software without restriction, including the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies.
 * The software is provided "as is", without warranty of any kind.
 */

#ifndef MCP9808_CFG_H
#define MCP9808_CFG_H

/**
 *  @brief Only MCP9808 temperature sensor configuration may be changed here.
 */

/** @section Defines */
#define MCP9808_I2C_TIMEOUT_MS (100U) /**< Timeout for I2C operations in milliseconds. */

#endif /* MCP9808_CFG_H */