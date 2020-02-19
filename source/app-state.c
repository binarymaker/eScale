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
      OLED_DISPLAY_SetPointer(31, 2);OLED_DISPLAY_Icon(encoderRuler_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(30, 1);OLED_DISPLAY_Printf("Encoder Tape");
      break;
    case 1:
      OLED_DISPLAY_SetPointer(31, 2);OLED_DISPLAY_Icon(TapeIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(33, 1);OLED_DISPLAY_Printf("Laser Tape");
      break;
    case 2:
      OLED_DISPLAY_SetPointer(31, 2);OLED_DISPLAY_Icon(protratorIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(48, 1);OLED_DISPLAY_Printf("Angle");
      break;
    case 3:
      OLED_DISPLAY_SetPointer(31, 2);OLED_DISPLAY_Icon(areaIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(50, 1);OLED_DISPLAY_Printf("Area");
      break;
    case 4:
      OLED_DISPLAY_SetPointer(31, 2);OLED_DISPLAY_Icon(volumeIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(44, 1);OLED_DISPLAY_Printf("Volume");
      break;
    case 5:
      OLED_DISPLAY_SetPointer(31, 2);OLED_DISPLAY_Icon(pythagoreamIcon_64x48, 64, 48);
      OLED_DISPLAY_SetPointer(34, 1);OLED_DISPLAY_Printf("Pythagoras");
      break;
    default:
      menu_id_u8 = menu_id_u8 % 5;
      //todo warning
      break;
  }
  OLED_DISPLAY_SetPointer(109, 0);OLED_DISPLAY_Printf("%d/5", menu_id_u8);
}

typedef enum
{
  APP_DISPLAY_UNIT_M,
  APP_DISPLAY_UNIT_CM,
  APP_DISPLAY_UNIT_FT,
  APP_DISPLAY_UNIT_IN,
}APP_DISPLAY_UNIT_et;

void
APP_ReadingDisplay(int32_t value_mm_i32, APP_DISPLAY_UNIT_et unit_ev)
{
  OLED_DISPLAY_FontSelect(SquareFont16x24, 16, 24, 43, 58);
  OLED_DISPLAY_SetPointer(0, 4);
  switch(unit_ev)
  {
    case APP_DISPLAY_UNIT_M:
      OLED_DISPLAY_Printf("%+02.3f", (float)value_mm_i32/1000);
      break;
    case APP_DISPLAY_UNIT_CM:
      OLED_DISPLAY_Printf("%+04.1f", (float)value_mm_i32/10);
      break;
    case APP_DISPLAY_UNIT_FT:
      OLED_DISPLAY_Printf("%+03.2f", (float)value_mm_i32 * 0.00328082);
      break;
    case APP_DISPLAY_UNIT_IN:
      OLED_DISPLAY_Printf("%+04.1f", (float)value_mm_i32 * 0.0393701);
      break;
    default:
      break;
  }
}
STATE_MACHINE_State(APP_MENU)
{
  escale_st * escale_ptr = (escale_st *) STATE_MACHINE_ptr;
  static uint8_t last_menu_select;
  
  if (STATE_ENTRY)
  {
    /* menu font select */
    OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
    last_menu_select = 100;
  }
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    if (5 < escale_ptr->encoder_nav.count)
    {
      ROTARY_ENCODER_SetCount(&escale_ptr->encoder_nav, 0);
    }
    if (0 > escale_ptr->encoder_nav.count)
    {
      ROTARY_ENCODER_SetCount(&escale_ptr->encoder_nav, 5);
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
        STATE_MACHINE_StateChange(APP_ENCODER_TAPE);
        break;
      case 1:
        STATE_MACHINE_StateChange(APP_LASER_TAPE);
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
      ROTARY_ENCODER_SetCount(&escale_ptr->encoder_tape, 0);
    }
  }
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    rotation_pulse_count = escale_ptr->encoder_tape.count;
  }
  
  if (GPIO_PinRead(P_B5) == LOW) //TODO BSP package
  {
    while(GPIO_PinRead(P_B5) == LOW);
    STATE_MACHINE_StateChange(APP_MENU);
  }
  /* menu font select */
  OLED_DISPLAY_FontSelect(SquareFont16x24, 16, 24, 43, 58);
  OLED_DISPLAY_SetPointer(15, 4);
  APP_ReadingDisplay(rotation_pulse_count, APP_DISPLAY_UNIT_M);
    
  if(STATE_EXIT)
  {
    
  }
}

STATE_MACHINE_State(APP_LASER_TAPE)
{
  if(STATE_ENTRY)
  {
    OLED_DISPLAY_SetPointer(0,0);
    OLED_DISPLAY_FillScreen(0x00);
    
    /* menu font select */
    OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
    OLED_DISPLAY_SetPointer(33, 1);
    OLED_DISPLAY_Printf("Laser Tape");
    OLED_DISPLAY_SetPointer(115, 5);
    OLED_DISPLAY_Printf("M");
  }

  if (GPIO_PinRead(P_B5) == LOW) //TODO BSP package
  {
    while(GPIO_PinRead(P_B5) == LOW);
    STATE_MACHINE_StateChange(APP_MENU);
  }
  
  if(STATE_EXIT)
  {
    
  }
}