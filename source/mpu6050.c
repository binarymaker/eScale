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

/* Includes ------------------------------------------------------------------*/
#include "mpu6050.h"
#include "mcu.h"
#include <math.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t reg_data[14];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void
MPU6050_Init(mpu6050_st * self, uint8_t dev_addr)
{
   uint8_t config_data;

  self->dev_addr  = dev_addr;
  
  config_data = 0x00;
  I2C_Transmit(self->dev_addr, 0x6B, &config_data, 1);
  
//  /* Configure the accelerometer (+/-8g) */
//  config_data = 0x10;
//  I2C_Transmit(self->dev_addr, 0x1C, &config_data, 1);
//
//  /* Configure the gyro (500dps full scale) */
//  config_data = 0x08;
//  I2C_Transmit(self->dev_addr, 0x1B, &config_data, 1);

}

void
MPU6050_Read(mpu6050_st * self)
{
  I2C_Receive(self->dev_addr, 0x3B, reg_data, 14);
  
  self->accl_data[0] = reg_data[0] << 8 | reg_data[1];
  self->accl_data[1] = reg_data[2] << 8 | reg_data[3];
  self->accl_data[2] = reg_data[4] << 8 | reg_data[5];
  
  self->gyro_data[0] = reg_data[8]  << 8 | reg_data[9];
  self->gyro_data[1] = reg_data[10] << 8 | reg_data[11];
  self->gyro_data[2] = reg_data[12] << 8 | reg_data[13];
  
}

void
MPU6050_calculateAngle(mpu6050_st * self)
{
  double x;
  double y;
  double z;
  
  double angle_x;
  double angle_y;
  double angle_z;
  
  x = self->accl_data[MPU6050_X_AXIS];
  y = self->accl_data[MPU6050_Y_AXIS];
  z = self->accl_data[MPU6050_Z_AXIS];
  
  angle_x = atan(x / sqrt((y * y) + (z * z)));
  angle_y = atan(y / sqrt((x * x) + (z * z)));
  angle_z = atan(z / sqrt((x * x) + (y * y)));
  
  self->angle[MPU6050_X_AXIS] = angle_x * (180.0 / 3.14);
  self->angle[MPU6050_Y_AXIS] = angle_y * (180.0 / 3.14);
  self->angle[MPU6050_Z_AXIS] = angle_z * (180.0 / 3.14);
}