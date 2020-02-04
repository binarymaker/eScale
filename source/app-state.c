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
#include "util/atomic.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t menu_select;
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
  OLED_DISPLAY_SetPointer(109, 0);OLED_DISPLAY_Printf("%d/5", menu_id_u8);
}

STATE_MACHINE_State(APP_MENU)
{
  escale_st * escale_ptr = (escale_st *) STATE_MACHINE_ptr;
  static uint8_t last_menu_select = 100;
  
  if (STATE_ENTRY)
  {
    /* menu font select */
    OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
  }
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    if (5 < escale_ptr->encoder_nav.count)
    {
      escale_ptr->encoder_nav.count = 0;
    }
    if (0 > escale_ptr->encoder_nav.count)
    {
      escale_ptr->encoder_nav.count = 5;
    }
    menu_select = escale_ptr->encoder_nav.count;
  }
  
  if(last_menu_select != menu_select)
  {
    APP_MenuDisplay(menu_select);
    last_menu_select = menu_select;
  }
  
  if (GPIO_PinRead(P_B5) == LOW) //TODO BSP package
  {
    while(GPIO_PinRead(P_B5) == LOW);
    switch(menu_select)
    {
      case 0:
        STATE_MACHINE_StateChange(APP_ENCODER_TAPE)
        break;
        
      default:
        break;
    }
  }
  
  if (STATE_EXIT)
  {

  }
}

STATE_MACHINE_State(APP_ENCODER_TAPE)
{
  escale_st * escale_ptr = (escale_st *) STATE_MACHINE_ptr;
  static int32_t rotation_pulse_count;
  if (STATE_ENTRY)
  {
    OLED_DISPLAY_SetPointer(0,0);
    OLED_DISPLAY_FillScreen(0x00);
    
    /* menu font select */
    OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
    OLED_DISPLAY_SetPointer(30, 1);
    OLED_DISPLAY_Printf("Encoder Tape");
    OLED_DISPLAY_SetPointer(115, 5);
    OLED_DISPLAY_Printf("M");
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      escale_ptr->encoder_tape.count = 0;
    }
  }
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    rotation_pulse_count = escale_ptr->encoder_tape.count;
    if(0 > rotation_pulse_count)
    {
      escale_ptr->encoder_tape.count = 0;
      rotation_pulse_count = 0;
    }
  }
  
  /* menu font select */
  OLED_DISPLAY_FontSelect(SquareFont16x24, 16, 24, 43, 58);
  OLED_DISPLAY_SetPointer(15, 4);
  OLED_DISPLAY_Printf("%06d", rotation_pulse_count);
    
  if(STATE_EXIT)
  {
    
  }
}