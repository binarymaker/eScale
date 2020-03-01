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
#include "mcu.h"
#include "oled-display.h"
#include "circular-buffer.h"
#include "app-state.h"
#include "icon-set.h"
#include "rotary-encoder.h"
#include "mpu6050.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t serial_buffer[32];
mpu6050_st angle_sensor;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int
main()
{
  MCU_Init();
  STATE_MACHINE_Init(escale.state_machine, APP_MENU);
  ROTARY_ENCODER_Init(&escale.encoder_tape, P_C0, P_C1);
  ROTARY_ENCODER_Init(&escale.encoder_nav, P_C2, P_C3);

  MPU6050_Init(&angle_sensor, 0x68);
  
  MCU_INTERRUPT_ENABLE();
  
  CIRCULAR_BUFFER_Init(&escale.serial_receiver, serial_buffer, sizeof(uint8_t), 32);
  
  OLED_DISPLAY_Init();
  OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);

  OLED_DISPLAY_SetPointer(0, 0);
  OLED_DISPLAY_Icon(eScaleLogo_128x64, 128, 64);
   
  SYSTIMER_Delay(1000);
  
  OLED_DISPLAY_SetPointer(0, 0);
  OLED_DISPLAY_FillScreen(0x00);

  while(1)
  {
    //STATE_MACHINE_Exec(escale.state_machine);
    
    MPU6050_Read(&angle_sensor);
    MPU6050_calculateAngle(&angle_sensor);
    
    OLED_DISPLAY_SetPointer(0, 0);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.accl_data[0]);
    OLED_DISPLAY_SetPointer(0, 1);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.accl_data[1]);
    OLED_DISPLAY_SetPointer(0, 2);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.accl_data[2]);
    
    OLED_DISPLAY_SetPointer(0, 4);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.gyro_data[0]);
    OLED_DISPLAY_SetPointer(0, 5);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.gyro_data[1]);
    OLED_DISPLAY_SetPointer(0, 6);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.gyro_data[2]);
    
    OLED_DISPLAY_SetPointer(64, 5);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.angle[0]);
    OLED_DISPLAY_SetPointer(64, 6);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.angle[1]);
    OLED_DISPLAY_SetPointer(64, 7);
    OLED_DISPLAY_Printf("%05d ", angle_sensor.angle[2]);
    
    DELAY_ms(100);
  } 
}
  
