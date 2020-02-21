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
static int8_t menu_select;
static int8_t option_select;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint32_t
APP_LaserSensorMeasure(circularBuffer_st * buffer_obj)
{
  uint32_t distance_mm_u32;
  uint8_t * serial_data_u8ptr = (uint8_t *) buffer_obj->buffer_ptr;

  CIRCULAR_BUFFER_Flush(buffer_obj);
  USART_Printf("AT1#");

  while(CIRCULAR_BUFFER_Available(buffer_obj) < 8){
    USART_Printf("\n buffer : %d", CIRCULAR_BUFFER_Available(buffer_obj));
    DELAY_ms(1000);
  }

  if (serial_data_u8ptr[0] == 'A' && serial_data_u8ptr[1] == 'T' &&
      serial_data_u8ptr[2] == 'D')
  {
    distance_mm_u32 = (uint32_t)serial_data_u8ptr[5]       |
                      (uint32_t)serial_data_u8ptr[4] << 8  |
                      (uint32_t)serial_data_u8ptr[3] << 16 ;
  }
  else
  {
    distance_mm_u32 = 0u;
  }
  
  return distance_mm_u32;
}

static void
APP_MenuDisplay(uint8_t menu_id_u8)
{
   OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
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

enum
{
  MEASURE, CLEAR, UNIT, EXIT, OFFSET
};

static void
APP_OptionDisplay(uint8_t option_id_u8)
{
  OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
  /* Clear option line in display */
  OLED_DISPLAY_SetPointer(40, 7) ;OLED_DISPLAY_Printf("           ");
  switch(option_id_u8)
  {
    case MEASURE:
     OLED_DISPLAY_SetPointer(40, 7);OLED_DISPLAY_Printf("[measure]");
     break;
    case CLEAR:
      OLED_DISPLAY_SetPointer(43, 7);OLED_DISPLAY_Printf("[clear]");
      break;
    case OFFSET:
      OLED_DISPLAY_SetPointer(40, 7);OLED_DISPLAY_Printf("[offset]");
      break;
    case UNIT:
      OLED_DISPLAY_SetPointer(46, 7);OLED_DISPLAY_Printf("[unit]");
      break;
    case EXIT:
      OLED_DISPLAY_SetPointer(46, 7);OLED_DISPLAY_Printf("[exit]");
      break;
    default:
      // TODO
      break;
  }
}

typedef enum
{
  APP_DISPLAY_UNIT_M,
  APP_DISPLAY_UNIT_CM,
  APP_DISPLAY_UNIT_FT,
  APP_DISPLAY_UNIT_IN,
}APP_DISPLAY_UNIT_et;

void
APP_UnitDisplay(APP_DISPLAY_UNIT_et unit_ev)
{
  OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
  OLED_DISPLAY_SetPointer(116, 5);
  switch(unit_ev)
  {
    case APP_DISPLAY_UNIT_M:
      OLED_DISPLAY_Printf("M ");
      break;
    case APP_DISPLAY_UNIT_CM:
      OLED_DISPLAY_Printf("CM");
      break;
    case APP_DISPLAY_UNIT_FT:
      OLED_DISPLAY_Printf("ft");
      break;
    case APP_DISPLAY_UNIT_IN:
      OLED_DISPLAY_Printf("in");
      break;
    default:
      break;
  }
}

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
    ROTARY_ENCODER_SetCount(&escale_ptr->encoder_nav, last_menu_select);
    last_menu_select = -1;
  }
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    ROTARY_ENCODER_LimitCount(&escale_ptr->encoder_nav, 0, 5);
    menu_select = ROTARY_ENCODER_ReadCount(&escale_ptr->encoder_nav);
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
  static uint8_t last_option_select;
  static int32_t rotation_pulse_count;
  static uint8_t unit_select;

  if (STATE_ENTRY)
  {
    OLED_DISPLAY_SetPointer(0,0);
    OLED_DISPLAY_FillScreen(0x00);
    
    /* menu font select */
    OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
    OLED_DISPLAY_SetPointer(30, 1);
    OLED_DISPLAY_Printf("Encoder Tape");
    APP_UnitDisplay(APP_DISPLAY_UNIT_M);

    option_select = CLEAR;
    last_option_select = -1;

    ROTARY_ENCODER_SetCount(&escale_ptr->encoder_tape, 0);
    ROTARY_ENCODER_SetCount(&escale_ptr->encoder_nav, option_select);
  }

  ROTARY_ENCODER_LimitCount(&escale_ptr->encoder_nav, CLEAR, EXIT);
  option_select = ROTARY_ENCODER_ReadCount(&escale_ptr->encoder_nav);

  if(last_option_select != option_select)
  {
    APP_OptionDisplay(option_select);
    last_option_select = option_select;
  }

  if (GPIO_PinRead(P_B5) == LOW) //TODO BSP package
  {
    while(GPIO_PinRead(P_B5) == LOW);
    switch (option_select)
    {
    case CLEAR:
      ROTARY_ENCODER_SetCount(&escale_ptr->encoder_tape, 0);
      break;
    case UNIT:
      unit_select++;
      if (APP_DISPLAY_UNIT_IN < unit_select)
      {
        unit_select = APP_DISPLAY_UNIT_M;
      }
      APP_UnitDisplay(unit_select);
      break;
    case EXIT:
      STATE_MACHINE_StateChange(APP_MENU);
      break;
    default:
      break;
    }
  }

  rotation_pulse_count = ROTARY_ENCODER_ReadCount(&escale_ptr->encoder_tape);

  APP_ReadingDisplay(rotation_pulse_count, unit_select);

  if(STATE_EXIT)
  {
    
  }
}

STATE_MACHINE_State(APP_LASER_TAPE)
{
  escale_st * escale_ptr = (escale_st *) STATE_MACHINE_ptr;
  static uint8_t last_option_select;
  static uint8_t unit_select;
  uint32_t distance_mm;

  if(STATE_ENTRY)
  {
    OLED_DISPLAY_SetPointer(0,0);
    OLED_DISPLAY_FillScreen(0x00);
    
    /* menu font select */
    OLED_DISPLAY_FontSelect(Font_6x8, 6, 8, 32, 127);
    OLED_DISPLAY_SetPointer(33, 1);
    OLED_DISPLAY_Printf("Laser Tape");
    APP_UnitDisplay(unit_select);
    
    option_select = MEASURE;
    last_option_select = -1;

    ROTARY_ENCODER_SetCount(&escale_ptr->encoder_nav, option_select);
  }

  ROTARY_ENCODER_LimitCount(&escale_ptr->encoder_nav, MEASURE, EXIT);
  option_select = ROTARY_ENCODER_ReadCount(&escale_ptr->encoder_nav);

  if(last_option_select != option_select)
  {
    APP_OptionDisplay(option_select);
    last_option_select = option_select;
  }

  if (GPIO_PinRead(P_B5) == LOW) //TODO BSP package
  {
    while(GPIO_PinRead(P_B5) == LOW);
    switch (option_select)
    {
    case MEASURE:
      distance_mm = APP_LaserSensorMeasure(&escale_ptr->serial_receiver);
      APP_ReadingDisplay(distance_mm, unit_select);
      break;
    case CLEAR:
      ROTARY_ENCODER_SetCount(&escale_ptr->encoder_tape, 0);
      break;
    case UNIT:
      unit_select++;
      if (APP_DISPLAY_UNIT_IN < unit_select)
      {
        unit_select = APP_DISPLAY_UNIT_M;
      }
      APP_UnitDisplay(unit_select);
      APP_ReadingDisplay(distance_mm, unit_select);
      break;
    case EXIT:
      STATE_MACHINE_StateChange(APP_MENU);
      break;
    default:
      break;
    }
  }
  
  if(STATE_EXIT)
  {
    
  }
}