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
  
  while(1)
  {
    OLED_DISPLAY_SetPointer(0, 1);
    OLED_DISPLAY_Printf("Laster dist(meter)");
    OLED_DISPLAY_SetPointer(0, 2);
    OLED_DISPLAY_Printf("Serial buffer %03d", CIRCULAR_BUFFER_Available(&serial_buffer_obj));
    DELAY_ms(1000);
  }
}
