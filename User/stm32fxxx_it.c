/**
  ******************************************************************************
  * @file    stm32fxxx_it
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32fxxx_it.h"
#include "main.h"
#include "Funciones.h"
#include "tm_stm32_disco.h"
#include "ALE_USART_F4.h"
#include "ALE_GPIO.h"
#include "USBfunc.h"
#include "packet.h"
#include "tm_stm32_usb_device.h"
#include "tm_stm32_usb_device_msc.h"
#include "tm_stm32_usb_device_cdc.h"
/** @addtogroup Template_Project
  * @{
  */

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern CAN_HandleTypeDef hcan2;
extern CanRxMsgTypeDef RxMessage;
extern USART_HandleTypeDef huart3;
extern USART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim7;
extern ALE_USART_Handle ahus;
extern ALE_USART_Handle ahus2;

extern uint32_t tiempo;
extern uint8_t  beacon;
extern uint16_t TPS, ECT, RPM, Lambda;
extern uint16_t IAT, BAT, VSS, ACCFUEL;
extern int SCHEDULER;
extern BUFFER BUFF;

extern uint8_t USBRead;
extern uint8_t telemetry_flag;
extern char telemetry_data[50];
extern uint8_t ADQOK;
extern uint32_t time;
extern uint8_t LCSFlag;

extern uint8_t TC_flag;

extern USB_MODE USB_SelectedMode;
extern USB_MODE USB_ActiveMode;
extern CONFIG_ADQ configADQ;

extern int8_t LaserOnESW;
extern CanTxMsgTypeDef TxMessage;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
	uint32_t LastLapTick=0;
	uint32_t LapTime=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4/7 Processor Exceptions Handlers                       */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	//TM_DISCO_LedOn(LED_BLUE);
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	//TM_DISCO_LedOn(LED_BLUE);
}

/**
  * @brief  This function decrement timing variable
  *	@with __weak parameter to prevent errors
  * @param  None
  * @retval None
  */
__weak void TimingDelay_Decrement(void) {

}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void) {
	/* Increase tick counter for HAL drivers */
//	if(USB_ActiveMode == USB_OFF && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_SET)
//	{
//		USB_ChangeMode(USB_SelectedMode);
//	} else if(USB_ActiveMode != USB_OFF && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET) {
//		USB_ChangeMode(USB_OFF);
//	}
	HAL_IncTick();
	
//	if(!(HAL_GetTick() % 10))
//	{
//		tiempo++;
//	  USBRead=1;
//	}
}

/******************************************************************************/
/*                 STM32Fxxx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

void TIM3_IRQHandler(void)
{	
	packet_timerfunc();		
	
//	if(time % (1000/configADQ.SD) == 0)
//	{
//	  USBRead=1;
//	}
	
	if(!(time % 50))
	{
		USBRead |= 0x01;
	}
	
	if(!(time % 10))
	{
		USBRead |= 0x02;
	}
	
	if(time % (1000/configADQ.TELEMETRY) == 0)
	{
		telemetry_flag=1;
	}
	
	time++;
	HAL_TIM_IRQHandler(&htim3);
}


void CAN2_RX0_IRQHandler(void)
{ 	
	HAL_CAN_IRQHandler(&hcan2);		//Maneja la interrupcion del bus CAN. Tambien lee la trama 
																//y lo carga en RxMessage (Conectada en el main)
	ReadCANData(&RxMessage);
	//Comprobar tiempo sin actualizar
	HAL_CAN_Receive_IT(&hcan2,CAN_FIFO0);
}

void CAN2_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan2);
}


void USART2_IRQHandler(void)
{
  ALE_USART_IRQHandler(&ahus2);
	HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
}

void USART3_IRQHandler(void)
{
  ALE_USART_IRQHandler(&ahus);
	HAL_NVIC_ClearPendingIRQ(USART3_IRQn);
}

void TIM5_IRQHandler(void)
{
	static int capture_number=0, period_counter=0;
	static uint32_t FirstValue=0, SecondValue=0, Frequency=0;
	static uint32_t lastTick = 0;
	
	
	if(__HAL_TIM_GET_FLAG(&htim5, TIM_FLAG_CC2) != RESET)
  {
		if(HAL_GetTick() - lastTick > 3000)
		{
			if(capture_number == 0)
			{
				FirstValue = HAL_TIM_ReadCapturedValue(&htim5,TIM_CHANNEL_2);
				capture_number = 1;
			}
			else if(capture_number == 1)
			{
				capture_number = 1; //Just for remember that in the next interrupt we'll execute this part of the code
				SecondValue = HAL_TIM_ReadCapturedValue(&htim5,TIM_CHANNEL_2);
				
				if (SecondValue > FirstValue)
				{
					Frequency = SecondValue - FirstValue;
					FirstValue=SecondValue;
				}
				else if (SecondValue < FirstValue)  //Just in case the TIM1 overflows
				{
					Frequency = ((0xFFFFFFFF - FirstValue) + SecondValue);
					FirstValue=SecondValue;
				}
				else
				{
					Frequency = 0;
					FirstValue=SecondValue;
				}
				
				if(Frequency<3000 && Frequency>2500) //3000 y 2500
				{
					if(++period_counter == 4)
					{
						beacon = 1;
						capture_number = 0;
						period_counter = 0;
						lastTick = HAL_GetTick();
						SendfromADQ(800,0,0,1);
						LapTime = lastTick - LastLapTick;
						LastLapTick = lastTick;
//						if (LaserOnESW==0)
//							{
//								addCan32Bit(LapTime, hcan2.pTxMsg,0);
//								SendCanData(6,800,&TxMessage,&hcan2);
//							}
						sendBeaconDetected(LapTime);
					}
				}
				else if(Frequency<1600 && Frequency>1300) //beacon #2 entre 1550 y 1400
				{
					if(++period_counter == 4)
					{
						beacon = 2;
						capture_number = 0;
						period_counter = 0;
						lastTick = HAL_GetTick();
						SendfromADQ(800,0,0,1);
						LapTime = lastTick - LastLapTick;
						LastLapTick = lastTick;
//						if (LaserOnESW==0)
//							{
//								addCan32Bit(LapTime, hcan2.pTxMsg,0);
//								SendCanData(6,800,&TxMessage,&hcan2);
//							}
						sendBeaconDetected(LapTime);
					}
				}
				else if(Frequency<1800 && Frequency>1700) //Beacon #3 entre 1600 y 1900
				{
					if(++period_counter == 4)
					{
						beacon = 3;
						capture_number = 0;
						period_counter = 0;
						lastTick = HAL_GetTick();
						SendfromADQ(800,0,0,1);
						LapTime = lastTick - LastLapTick;
						LastLapTick = lastTick;
//						if (LaserOnESW==0)
//							{
//								addCan32Bit(LapTime, hcan2.pTxMsg,0);
//								SendCanData(6,800,&TxMessage,&hcan2);
//							}
						sendBeaconDetected(LapTime);
					}
				}
				else if(Frequency<6300 && Frequency>6100) //Beacon #4 entre 6100 y 6300
				{
					if(++period_counter == 4)
					{
						beacon = 4;
						capture_number = 0;
						period_counter = 0;
						lastTick = HAL_GetTick();
						SendfromADQ(800,0,0,1);
						LapTime = lastTick - LastLapTick;
						LastLapTick = lastTick;
//						if (LaserOnESW==0)
//							{
//								addCan32Bit(LapTime, hcan2.pTxMsg,0);
//								SendCanData(6,800,&TxMessage,&hcan2);
//							}
						sendBeaconDetected(LapTime);
					}
				}
				else
				{
					capture_number = 0;
					period_counter = 0;
				}
			}
		}
	}
  HAL_TIM_IRQHandler(&htim5);
}

void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
	
//	if(USB_ActiveMode == USB_OFF)
//	{
//		USB_ChangeMode(USB_SelectedMode);
//	} else {
//		USB_ChangeMode(USB_OFF);
//	}
	
  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
