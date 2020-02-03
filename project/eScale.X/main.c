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
  STATE_MACHINE_Init(escale.state_machine, APP_MENU);

  MCU_INTERRUPT_ENABLE();
  
  CIRCULAR_BUFFER_Init(&serial_buffer_obj, serial_buffer, sizeof(uint8_t), 32);
  
  OLED_DISPLAY_Init();
  OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);

  OLED_DISPLAY_SetPointer(0, 0);
  OLED_DISPLAY_Icon(eScaleLogo_128x64, 128, 64);
  SYSTIMER_Delay(5000);

  OLED_DISPLAY_SetPointer(0, 0);
  OLED_DISPLAY_FillScreen(0x00);

  while(1)
  {
    STATE_MACHINE_Exec(escale.state_machine);
    DELAY_ms(2000);
  }
}
