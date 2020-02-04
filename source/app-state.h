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

#ifndef APP_STATE_6604f2da_4640_11ea_b50b_705a0f25cb51
#define APP_STATE_6604f2da_4640_11ea_b50b_705a0f25cb51

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * \brief Source file version tag
 *        
 *        version info: [15:8] main [7:0] beta
 */
#define __APP_STATE_VERSION      (0x0001u)

/* Includes ------------------------------------------------------------------*/
#include "state-machine.h"
#include "rotary-encoder.h"
/* Exported types ------------------------------------------------------------*/
typedef struct escale
{
  stateMachine_st state_machine;
  rotaryEncoder_st encoder_nav;
  rotaryEncoder_st encoder_tape;
}escale_st;

escale_st escale;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
STATE_MACHINE_State(APP_MENU);
STATE_MACHINE_State(APP_ENCODER_TAPE);

#ifdef __cplusplus
}
#endif

#endif /* APP_STATE_6604f2da_4640_11ea_b50b_705a0f25cb51 */

