/**\cond
  ******************************************************************************
  * ______  _                             ___  ___        _               
  * | ___ \(_)                            |  \/  |       | |              
  * | |_/ / _  _ __    __ _  _ __  _   _  | .  . |  __ _ | | __ ___  _ __ 
  * | ___ \| || '_ \  / _` || '__|| | | | | |\/| | / _` || |/ // _ \| '__|
  * | |_/ /| || | | || (_| || |   | |_| | | |  | || (_| ||   <|  __/| |   
  * \____/ |_||_| |_| \__,_||_|    \__, | \_|  |_/ \__,_||_|\_\\___||_|   
  *                                 __/ |                                 
  *                                |___/                                  
  *                                                                       
  * Copyright (C) 2020 Binary Maker - All Rights Reserved
  *
  * This program and the accompanying materials are made available
  * under the terms described in the LICENSE file which accompanies
  * this distribution.
  * Written by Binary Maker <https://github.com/binarymaker>
  ******************************************************************************
  \endcond*/

#ifndef MPU6050_8d3f2ef0_5646_11ea_a408_0026b912d7d8
#define MPU6050_8d3f2ef0_5646_11ea_a408_0026b912d7d8

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * \brief Source file version tag
 *        
 *        version info: [15:8] main [7:0] beta
 */
#define __MPU6050_VERSION      (0x0001u)

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "common-def.h"
/* Exported types ------------------------------------------------------------*/
typedef struct mpu6050_s
{
  uint8_t dev_addr;
  int16_t accl_data[3];
  int16_t gyro_data[3];
  int16_t angle[3];
}mpu6050_st;

/* Exported constants --------------------------------------------------------*/
enum
{
  MPU6050_X_AXIS, MPU6050_Y_AXIS, MPU6050_Z_AXIS,
};
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void
MPU6050_Init(mpu6050_st * self, uint8_t dev_addr);

void
MPU6050_Read(mpu6050_st * self);

void
MPU6050_calculateAngle(mpu6050_st * self);

#ifdef __cplusplus
}
#endif

#endif /* MPU6050_8d3f2ef0_5646_11ea_a408_0026b912d7d8 */

