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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
circularBuffer_st serial_buffer_obj;
uint8_t serial_buffer[32];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int
main()
{
  MCU_Init();
  
  MCU_INTERRUPT_ENABLE();
  
  CIRCULAR_BUFFER_Init(&serial_buffer_obj, serial_buffer, sizeof(uint8_t), 32);
  OLED_DISPLAY_Init();
  OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
  
  OLED_DISPLAY_FillScreen(0x00);
  
  OLED_DISPLAY_SetPointer(46, 1);
  OLED_DISPLAY_Printf("eScale");
  OLED_DISPLAY_SetPointer(10, 2);
  OLED_DISPLAY_Printf("Measure everything");
  SYSTIMER_Delay(1000);
  
  OLED_DISPLAY_FillScreen(0x00);
  
  uint8_t laser_sensor_reply[7];
  uint32_t distance;
  while(1)
  {
    /* Clear serial datas */
    CIRCULAR_BUFFER_Flush(&serial_buffer_obj);
    /* Measure commend to sensor */
    USART_Printf("AT1#");
    while(CIRCULAR_BUFFER_Available(&serial_buffer_obj) < 7)
    {
      /* wail for sensor reply */
    }
    uint8_t idx_u8 = 0;
    uint8_t data;
    while(CIRCULAR_BUFFER_Available(&serial_buffer_obj))
    {
      CIRCULAR_BUFFER_Read(&serial_buffer_obj, &data);
      laser_sensor_reply[idx_u8] = data;
      idx_u8++;
    }
    
    if (laser_sensor_reply[0] == (uint8_t)'A')
    {
      /* 0 1 2 3 4 5 6
       * A T D $ $ $ C
       * $ is data sequence
       */
      distance = (uint32_t)laser_sensor_reply[5]       | 
                 (uint32_t)laser_sensor_reply[4] << 8  |
                 (uint32_t)laser_sensor_reply[3] << 16 ;
      
      OLED_DISPLAY_SetPointer(0, 2);
      OLED_DISPLAY_Printf("dist(meter) = %6d", distance);
    }
    DELAY_ms(3000);
  }
}
