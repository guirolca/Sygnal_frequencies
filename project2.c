#include "Config.h"
#include "stm32f4xx_hal.h"
#include "mxconstants.h"
#include "ALE_USART_F4.h"
#include "ALE_GPIO_F4.h"
#include "tm_stm32_usb_host.h"
#include "tm_stm32_usb_device.h"
#include "tm_stm32_usb_device_msc.h"
#include "tm_stm32_usb_host_msc.h"
#include "tm_stm32_fatfs.h"
#include "tm_stm32_rtc.h"
#include "Funciones.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern CAN_HandleTypeDef hcan2;
extern CAN_FilterConfTypeDef hcan2Filter;
extern USART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim7;
extern ALE_USART_Handle ahus;

extern TM_RTC_t RTCD;

extern float kp,ki,kd;
extern uint32_t LCSMAXSPEED;
extern uint16_t RPMTARGET;

extern uint32_t shift_cut, shift_up, shift_down;

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	
  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 10;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
	
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

void MX_GPIO_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
	
//	GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);
	
//	GPIO_InitStruct.Pin = VBUS_FS_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);
//	
//	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
	
	#ifdef LED
		GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	#endif

  LedGreenOff;
}

void TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10-1; //500 1seg
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
}


void TIM5_Init(void)  //Beicon timer input capture
{
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 84-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xFFFFFFFF;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_IC_Init(&htim5);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_2);
}


void MX_CAN2_Init(void)
{
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 3;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SJW = CAN_SJW_1TQ;
  hcan2.Init.BS1 = CAN_BS1_6TQ;
  hcan2.Init.BS2 = CAN_BS2_7TQ;
  hcan2.Init.TTCM = DISABLE;
  hcan2.Init.ABOM = DISABLE;
  hcan2.Init.AWUM = DISABLE;
  hcan2.Init.NART = DISABLE;
  hcan2.Init.RFLM = DISABLE;
  hcan2.Init.TXFP = DISABLE;
  HAL_CAN_Init(&hcan2);
	hcan2Filter.FilterNumber = 0; 										
	hcan2Filter.FilterMode = CAN_FILTERMODE_IDMASK;					
	hcan2Filter.FilterScale = CAN_FILTERSCALE_32BIT;					// Specifies the filter scale.
	hcan2Filter.FilterIdHigh = 0x00;											
	hcan2Filter.FilterIdLow = 0x00;											
	hcan2Filter.FilterMaskIdHigh = 0x00;										
	hcan2Filter.FilterMaskIdLow = 0x00;										
	hcan2Filter.FilterFIFOAssignment = 0;									// Specifies the FIFO (0 or 1) which will be assigned to the filter.
	hcan2Filter.FilterActivation = ENABLE;									// Enable or disable the filter.
	hcan2Filter.BankNumber = 0;											// Select the start slave bank filter
	HAL_CAN_ConfigFilter(&hcan2, &hcan2Filter);
}

void UARTTelemetry_Config(void)
{
  ahus.baudrate=115200;
	ahus.GPIO_Rx=GPIOC;
	ahus.GPIO_Tx=GPIOB;
	ahus.GPIO_Rx_pin=GPIO_Pin_5;
	ahus.GPIO_Tx_pin=GPIO_Pin_10;
	ahus.USARTx = USART3;
	ahus.process_byte_func = processTelemetryByte;//RxTelemetry;
	ALE_USART_Init(&ahus,&huart3);
}

void RxTelemetry(unsigned char c)
{
	static uint32_t state=0,data=0;
	char feedback[25];
	switch(state)
	{
		case 0:
				if(c=='P') //For kp
				{
					state=1;
				}
				if(c=='I') //For ki
				{
					state=2;
				}
				if(c=='R') //For RPMTARGET
				{
					state=3;
				}
				if(c=='S') //For LCSMAXSPEED
				{
					state=4;
				}
				if(c=='D') //For kd
				{
					state=5;
				}
				if(c=='U') //For shift up
				{
					state=6;
				}
				if(c=='C') //For shift cut
				{
					state=7;
				}
		break;
		case 1:
			  if(c=='E')
				{
					kp=(float)data/100;
					data=0;
					state=0;
					sprintf(feedback,"kp=%.2f\n",kp);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;
		case 2:
			  if(c=='E')
				{
					ki=(float)data/100;
					data=0;
					state=0;
					sprintf(feedback,"ki=%.2f\n",ki);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;	
		case 3:
			  if(c=='E')
				{
					RPMTARGET=data;
					data=0;
					state=0;
					sprintf(feedback,"RPM=%.2u\n",RPMTARGET);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;
		case 4:
			  if(c=='E')
				{
					LCSMAXSPEED=(float)data;
					data=0;
					state=0;
					sprintf(feedback,"MAXSPEED=%.2u\n",LCSMAXSPEED);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;
		case 5:
			  if(c=='E')
				{
					kd=(float)data/100;
					data=0;
					state=0;
					sprintf(feedback,"kd=%.2f\n",kd);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;
		case 6:
			  if(c=='E')
				{
					shift_up=data;
					data=0;
					state=0;
					sprintf(feedback,"SHIFTUP=%u ms\n",shift_up/10);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;
		case 7:
			  if(c=='E')
				{
					shift_cut=data;
					data=0;
					state=0;
					sprintf(feedback,"SHIFTCUT=%u ms\n",shift_cut/10);
					ALE_USART_Puts(&ahus,feedback);
				}
				else
				{
					data=(data*10)+(c-0x30);
				}
		break;
	}	
}

void USB_Config(void)
{
	TM_USB_Init();
	/* Init USB Host, FS and HS modes */
	TM_USBH_Init(TM_USB_FS);
	/* Enable MSC HOST class for FS and HS */
	TM_USBH_MSC_Init(TM_USB_FS);
	/* Start USB host on FS and HS */
	TM_USBH_Start(TM_USB_FS);
}


void RTC_Config(void)
{
	if (TM_RTC_Init(TM_RTC_ClockSource_External))  //RTC was already initialized and time is running
	{
 
  } 
	else //RTC was not initialized
	{
		   RTCD.Day = 1;
       RTCD.Month = 8;
       RTCD.Year = 16;
       RTCD.WeekDay = 1;
       RTCD.Hours =21;
       RTCD.Minutes = 50;
       RTCD.Seconds = 00;
       TM_RTC_SetDateTime(&RTCD, TM_RTC_Format_BIN);
  }
	
  TM_RTC_Interrupts(TM_RTC_Int_1s);
	TM_RTC_GetDateTime(&RTCD, TM_RTC_Format_BIN);
}
