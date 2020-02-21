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
  * Copyright (C) 2019 Binary Maker - All Rights Reserved
  *
  * This program and the accompanying materials are made available
  * under the terms described in the LICENSE file which accompanies
  * this distribution.
  * Written by Binary Maker <https://github.com/binarymaker>
  ******************************************************************************
  \endcond*/

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"
#include "circular-buffer.h"
#include "app-state.h"
#include "rotary-encoder.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern escale_st escale;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

ISR(TIMER0_OVF_vect)
{  
  SYSTIMER_Engin();
  BIT_Clear(TIFR0, TOV0);
}

ISR(USART_RX_vect)
{  
  uint8_t data_u8;
  data_u8 = USART_Read();
  CIRCULAR_BUFFER_Write(&escale.serial_receiver, &data_u8);
}

ISR(PCINT1_vect)
{
  ROTARY_ENCODER_Update(&escale.encoder_tape);
  ROTARY_ENCODER_Update(&escale.encoder_nav);
}