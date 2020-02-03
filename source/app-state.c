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
#include "app-state.h"
#include "oled-display.h"
#include "state-machine.h"
#include "icon-set.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t encoder_count;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void
APP_MenuDisplay(uint8_t menu_id_u8)
{
  /* Clear display */
  OLED_DISPLAY_SetPointer(0, 0) ;OLED_DISPLAY_FillScreen(0x00);
  
  /* print menu icon and title*/
  switch(menu_id_u8)
  {
    case 0:
      OLED_DISPLAY_SetPointer(31, 0);OLED_DISPLAY_Icon(encoderRuler_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(30, 6);OLED_DISPLAY_Printf("Encoder Tape");
      break;
    case 1:
      OLED_DISPLAY_SetPointer(31, 0);OLED_DISPLAY_Icon(TapeIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(30, 6);OLED_DISPLAY_Printf("Laser Meter");
      break;
    case 2:
      OLED_DISPLAY_SetPointer(31, 0);OLED_DISPLAY_Icon(protratorIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(30, 6);OLED_DISPLAY_Printf("Angle Meter");
      break;
    case 3:
      OLED_DISPLAY_SetPointer(31, 0);OLED_DISPLAY_Icon(areaIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(34, 6);OLED_DISPLAY_Printf("Area Meter");
      break;
    case 4:
      OLED_DISPLAY_SetPointer(31, 0);OLED_DISPLAY_Icon(volumeIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(30, 6);OLED_DISPLAY_Printf("Volume Meter");
      break;
    case 5:
      OLED_DISPLAY_SetPointer(31, 0);OLED_DISPLAY_Icon(pythagoreamIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(24, 6);OLED_DISPLAY_Printf("Pythagrom Meter");
      break;
    default:
      menu_id_u8 = menu_id_u8 % 5;
      //todo warning
      break;
  }
}

STATE_MACHINE_State(APP_MENU)
{
  static uint8_t last_encoder_count = 0;
  
  if (STATE_ENTRY)
  {

  }
  
  encoder_count++;
  encoder_count = encoder_count % 5;
  
  if(last_encoder_count != encoder_count)
  {
    APP_MenuDisplay(encoder_count);
    last_encoder_count = encoder_count;
  }

  if (STATE_EXIT)
  {

  }
}