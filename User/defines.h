/**
 * Defines for your entire project at one place
 * 
 * @author     Tilen Majerle
 * @email      tilen@majerle.eu
 * @website    http://stm32f4-discovery.com
 * @version    v1.0
 * @ide        Keil uVision 5
 * @license    GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Put your global defines for all libraries here used in your project */

/* Defines for RCC settings for system */
/* I've added these defines in options for target in Keil uVision for each target different settings */
//#define RCC_OSCILLATORTYPE    RCC_OSCILLATORTYPE_HSE /*!< Used to select system oscillator type */
//#define RCC_PLLM              8                      /*!< Used for PLL M parameter */
//#define RCC_PLLN              360                    /*!< Used for PLL N parameter */
//#define RCC_PLLP              2                      /*!< Used for PLL P parameter */
//#define RCC_PLLQ              7                      /*!< Used for PLL Q parameter */
//#define RCC_PLLR              10                     /*!< Used for PLL R parameter, available on STM32F446xx */

#define ADQ
#define CONTROL
//#define LED
#define BEACON

/* Disable cache */
#define DISABLE_CACHE
#define STM32F4xx
#define FATFS_USE_USB           1
#define USB_USE_FS
#define USB_USE_HOST
#define USB_USE_STM32F4_DISCOVERY

#define DATASIZE1 137
#define DATASIZE2 25
#define BLOCKSIZE 1024
#define BUFFERSIZE 4096
#define FILESIZE 10857600

#define _DISABLE_INTERRUPTS DisableAllInterrupts()
#define _ENABLE_INTERRUPTS EnableAllInterrupts()

#define ToggleLedGreen HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5)
#define LedGreenOn  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define LedGreenOff  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)

#define CUT_ON HAL_GPIO_WritePin(IGNOUT_GPIO_Port,IGNOUT4_Pin,GPIO_PIN_RESET)
#define CUT_OFF HAL_GPIO_WritePin(IGNOUT_GPIO_Port,IGNOUT4_Pin,GPIO_PIN_SET)

#define SHIFT_UP_ON HAL_GPIO_WritePin(SHIFT_GPIO_Port,SHOUT1_Pin,GPIO_PIN_SET)
#define SHIFT_UP_OFF HAL_GPIO_WritePin(SHIFT_GPIO_Port,SHOUT1_Pin,GPIO_PIN_RESET)
#define SHIFT_DN_ON HAL_GPIO_WritePin(SHIFT_GPIO_Port,SHOUT2_Pin,GPIO_PIN_SET)
#define SHIFT_DN_OFF HAL_GPIO_WritePin(SHIFT_GPIO_Port,SHOUT2_Pin,GPIO_PIN_RESET)


#define MAXINTEGRAL 500.0f
#define MININTEGRAL 0.0f

/***** Backup SRAM Memory Map *****/
/* AUX_FRONT */
#define AF_MLX90614_FREQ_ADDRESS 0
#define AF_CAN_TRANSMIT_FREQ_ADDRESS 2
#define AF_CAN_DAMPER_FREQ_ADDRESS 4
#define AF_GPS_READ_DATA_FREQ_ADDRESS 6

/* AUX_REAR */
#define AR_MLX90614_FREQ_ADDRESS 20
#define AR_CAN_TRANSMIT_FREQ_ADDRESS 22
#define AR_CAN_DAMPER_FREQ_ADDRESS 24

/* ESW */
#define ESW_DISPLAY_UPDATE_FREQ_ADDRESS 40
#define ESW_TACHOMETER_BLINK_FREQ_ADDRESS 42
#define ESW_ADQ_TIMEOUT_ADDRESS 44

/* ADQ */
#define ADQ_SD_WRITE_FREQ_ADDRESS 60
#define ADQ_TELEMETRY_FREQ_ADDRESS 62

#endif
