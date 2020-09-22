/**************************************************************************************************

  Phyplus Microelectronics Limited confidential and proprietary.
  All rights reserved.

  IMPORTANT: All rights of this software belong to Phyplus Microelectronics
  Limited ("Phyplus"). Your use of this Software is limited to those
  specific rights granted under  the terms of the business contract, the
  confidential agreement, the non-disclosure agreement and any other forms
  of agreements as a customer or a partner of Phyplus. You may not use this
  Software unless you agree to abide by the terms of these agreements.
  You acknowledge that the Software may not be modified, copied,
  distributed or disclosed unless embedded on a Phyplus Bluetooth Low Energy
  (BLE) integrated circuit, either as a product or is integrated into your
  products.  Other than for the aforementioned purposes, you may not use,
  reproduce, copy, prepare derivative works of, modify, distribute, perform,
  display or sell this Software and/or its documentation for any purposes.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  PHYPLUS OR ITS SUBSIDIARIES BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

**************************************************************************************************/

/*******************************************************************************
* @file		voice.h
* @brief	Contains all functions support for voice driver
* @version	0.0
* @date		18. Jun. 2018
* @author	qing.han
*
* Copyright(C) 2018, PhyPlus Microelectronics
* All rights reserved.
*
*******************************************************************************/
#ifndef __VOICE__H__
#define __VOICE__H__

#include "types.h"
#include "gpio.h"

#define    MAX_VOICE_SAMPLE_SIZE		512
#define    MAX_VOICE_SAMPLE_ID			(MAX_VOICE_SAMPLE_SIZE-1)
#define    HALF_VOICE_SAMPLE_SIZE		256
#define    HALF_VOICE_SAMPLE_ID			(HALF_VOICE_SAMPLE_SIZE-1)
#define    MAX_VOICE_WORD_SIZE			256
#define    HALF_VOICE_WORD_SIZE			128

//#define    BELOW_1V                 0
//#define    UP_1V                    1
//#define    ADC_INPUT_LEVEL          BELOW_1V
//#define    ADC_INPUT_LEVEL          UP_1V


#define    VOICE_BASE             (0x40050800UL)
#define    VOICE_MID_BASE         (0x40050a00UL)
#define    VOICE_SAMPLE_AUTO              0
#define    VOICE_SAMPLE_MANNUAL           1
#define    VOICE_SAMPLE_MODE              SAMPLE_MANNUAL
//#define    ADC_SAMPLE_MODE              SAMPLE_AUTO

#define    ENABLE_VOICE_INT      *(volatile unsigned int *)0x40050034 |= 0x00000300
//#define    ENABLE_VOICE_INT      *(volatile unsigned int *)0x40050034 &= 0xfffffcff
#define    MASK_VOICE_INT        *(volatile unsigned int *)0x40050034 &= 0xfffffcff
//#define    MASK_VOICE_INT        *(volatile unsigned int *)0x40050034 |= 0x00000300
/*#define    ADC_IRQ_ENABLE      *(volatile unsigned int *) 0xe000e100 = BIT(29)
#define    ADC_IRQ_DISABLE     *(volatile unsigned int *) 0xe000e100 = BIT(29)
#define    CLEAR_ADC_INT_CH0   *(volatile unsigned int *)0x40050038 |= BIT(0)
#define    CLEAR_ADC_INT_CH1   *(volatile unsigned int *)0x40050038 |= BIT(1)
#define    CLEAR_ADC_INT_CH2   *(volatile unsigned int *)0x40050038 |= BIT(2)
#define    CLEAR_ADC_INT_CH3   *(volatile unsigned int *)0x40050038 |= BIT(3)
#define    CLEAR_ADC_INT_CH4   *(volatile unsigned int *)0x40050038 |= BIT(4)
#define    CLEAR_ADC_INT_CH5   *(volatile unsigned int *)0x40050038 |= BIT(5)
#define    CLEAR_ADC_INT_CH6   *(volatile unsigned int *)0x40050038 |= BIT(6)
#define    CLEAR_ADC_INT_CH7   *(volatile unsigned int *)0x40050038 |= BIT(7)*/
#define    CLEAR_VOICE_HALF_INT *(volatile unsigned int *)0x40050038 |= BIT(8)
#define    CLEAR_VOICE_FULL_INT *(volatile unsigned int *)0x40050038 |= BIT(9)
/*#define    CLEAR_ADC_INT(n)    *(volatile unsigned int *)0x40050038 |= BIT(n)

#define    IS_CLAER_ADC_INT_CH0 (*(volatile unsigned int *)0x4005003c) & BIT(0)
#define    IS_CLAER_ADC_INT_CH1 (*(volatile unsigned int *)0x4005003c) & BIT(1)
#define    IS_CLAER_ADC_INT_CH2 (*(volatile unsigned int *)0x4005003c) & BIT(2)
#define    IS_CLAER_ADC_INT_CH3 (*(volatile unsigned int *)0x4005003c) & BIT(3)
#define    IS_CLAER_ADC_INT_CH4 (*(volatile unsigned int *)0x4005003c) & BIT(4)
#define    IS_CLAER_ADC_INT_CH5 (*(volatile unsigned int *)0x4005003c) & BIT(5)
#define    IS_CLAER_ADC_INT_CH6 (*(volatile unsigned int *)0x4005003c) & BIT(6)
#define    IS_CLAER_ADC_INT_CH7 (*(volatile unsigned int *)0x4005003c) & BIT(7)*/
#define    IS_CLAER_VOICE_HALF_INT (*(volatile unsigned int *)0x4005003c) & BIT(8)
#define    IS_CLAER_VOICE_FULL_INT (*(volatile unsigned int *)0x4005003c) & BIT(9)
//#define    IS_CLAER_ADC_INT(n)   (*(volatile unsigned int *)0x4005003c) & BIT(n)

#define    GET_IRQ_STATUS         ((*(volatile unsigned int *) 0x4005003c) & 0x3ff)
#define    ENABLE_ADC             (*(volatile unsigned int *)0x4000f048 |= BIT(3))
#define    DISABLE_ADC            (*(volatile unsigned int *)0x4000f048 &= ~BIT(3))
#define    ADC_CLOCK_ENABLE       (*(volatile unsigned int *)0x4000f044 |= BIT(13))
#define    ADC_CLOCK_DISABLE       (*(volatile unsigned int *)0x4000f044 &= ~BIT(13))
#define    ADC_DBLE_CLOCK_DISABLE (*(volatile unsigned int *)0x4000f044 &= ~BIT(21))
#define    POWER_DOWN_ADC         (*(volatile unsigned int *)0x4000f048 &= ~BIT(3))
#define    POWER_UP_TEMPSENSOR    (*(volatile unsigned int *)0x4000f048 |= BIT(29))
#define    REG_IO_CONTROL         ((volatile unsigned int  *)0x4000f020)



#define ADCC_REG_BASE   (0x4000F000)

// Voice encode mode
typedef enum {
	VOICE_ENCODE_PCMA = 0,
	VOICE_ENCODE_PCMU = 1,
	VOICE_ENCODE_CVSD = 2,
	VOICE_ENCODE_BYP = 3
}VOICE_ENCODE_t;

// Voice sample rate
typedef enum {
	VOICE_RATE_64K = 0,
	VOICE_RATE_32K = 1,
	VOICE_RATE_16K = 2,
	VOICE_RATE_8K = 3
}VOICE_RATE_t;

// Voice notch filter configuration
typedef enum {
	VOICE_NOTCH_BYP = 0,
	VOICE_NOTCH_1	= 1,
	VOICE_NOTCH_2	= 2,
	VOICE_NOTCH_3	= 3
}VOICE_NOTCH_t;

// Voice polarity selection
typedef enum {
	VOICE_POLARITY_POS = 0,
	VOICE_POLARITY_NEG = 1
}VOICE_POLARITY_t;

enum{
  HAL_VOICE_EVT_DATA = 1,
  HAL_VOICE_EVT_FAIL = 0xff
};

// Voice configuration structure
typedef struct _voice_Cfg_t{
	bool						voiceSelAmicDmic;
	GPIO_Pin_e 			dmicDataPin;
	GPIO_Pin_e 			dmicClkPin;
	uint8_t					amicGain;
	uint8_t					voiceGain;
	VOICE_ENCODE_t	voiceEncodeMode;
	VOICE_RATE_t 		voiceRate;
	bool						voiceAutoMuteOnOff;
}voice_Cfg_t;

// Voice event structure
typedef struct _voice_Evt_t{
  int       type;
  uint32_t* data;
  uint32_t  size;
}voice_Evt_t;

typedef void (*voice_Hdl_t)(voice_Evt_t* pev);

// Voice context structure
typedef struct _voice_Contex_t{
  bool      		enable;
	voice_Cfg_t		cfg;
  voice_Hdl_t		evt_handler;
}voice_Ctx_t;

void phy_voice_init(void);

// Enable voice core
void phy_voice_enable(void);

// Disable voice core
void phy_voice_disable(void);

// Select DMIC
void phy_voice_dmic_mode(void);

// Select AMIC
void phy_voice_amic_mode(void);

// Open a GPIO pin for DMIC
void phy_voice_dmic_open(GPIO_Pin_e dmicDataPin, GPIO_Pin_e dmicClkPin);

// Set PGA gain for AMIC
void phy_voice_amic_gain(uint8_t amicGain);

// Set voice process gain
void phy_voice_gain(uint8_t voiceGain);

// Set voice encoding mode
void phy_voice_encode(VOICE_ENCODE_t voiceEncodeMode);

// Set voice data rate
void phy_voice_rate(VOICE_RATE_t voiceRate);

// Enable voice auto-mute
void phy_voice_amute_on(void);

// Disable voice auto-mute
void phy_voice_amute_off(void);

/**************************************************************************************
 * @fn          phy_VOICE_IRQHandler
 *
 * @brief       This function process for adc interrupt
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
// Voice interrupt handler
void __attribute__((weak)) phy_ADC_IRQHandler(void);

/**************************************************************************************
 * @fn          phy_voice_init
 *
 * @brief       This function process for adc initial
 *
 * input parameters
 *
 * @param       ADC_MODE_e mode: adc sample mode select;1:SAM_MANNUAL(mannual mode),0:SAM_AUTO(auto mode)
 *              ADC_CH_e adc_pin: adc pin select;ADC_CH0~ADC_CH7 and ADC_CH_VOICE
 *              ADC_SEMODE_e semode: signle-ended mode negative side enable; 1:SINGLE_END(single-ended mode) 0:DIFF(Differentail mode)
 *              IO_CONTROL_e amplitude: input signal amplitude, 0:BELOW_1V,1:UP_1V
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 **************************************************************************************/
// Allocate memory and power manager for voice
void phy_voice_init(void);

// Configure voice capture
int phy_voice_config(voice_Cfg_t cfg, voice_Hdl_t evt_handler);

// Start voice capture
int phy_voice_start(void);

// Stop voice capture
int phy_voice_stop(void);

// Clear memory and power manager for voice
int phy_voice_clear(void);

#endif
