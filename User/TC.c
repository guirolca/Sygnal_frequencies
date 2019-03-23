#include "stm32f4xx_hal.h"
#include "TC.h"
#include "defines.h"



extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;

extern uint8_t GEAR;

volatile uint32_t cut_time=0; 
volatile uint8_t TC_flag=0, ShiftUP_ON=0, ShiftDN_ON=0, TC_CUT_ON=0, FTS_delay_ON=0;

volatile uint8_t shift_up_req=0, shift_dn_req=0, TC_req=0;
volatile uint32_t TC_cut_time_req=0;
volatile uint16_t SR_req=0,SR_Raw=0;
volatile uint8_t TC_ON_OFF=0;


const uint16_t Gear_Cut_Time[]={600,600,600,600,600,600};
const uint16_t Gear_Actuator_Time[]={3400,3400,3400,3400,3400,3400};
const uint32_t FTS_cut_delay=0; //5 ms (50)


void TC_CUTV1(float SR, uint8_t SR_target, float rpm_ms)
{	
	if(SR>(float)SR_target)
	{
		SR=SR-(float)SR_target;
		cut_time=(COMBUSTION_G*rpm_ms);
		TC_cut_time_req=cut_time;
		SR_req=(uint16_t)SR;
		if(TC_ON_OFF)
		{
			if(cut_time>=500)  //50ms (1/Fs)
			{
				TC_CUT_ON=1;
				TIM_CUT_Set(0,500,&htim4);
				SPARK_CUT_ON;
				TC_req=1;
			}
			else
			{
				TC_CUT_ON=1;
				TIM_CUT_Set(0,cut_time,&htim4);
				SPARK_CUT_ON;
				TC_req=1;
			}
		}
	}
}



void TC_ON(uint16_t speedFL, uint16_t speedFR, uint16_t speedRL, uint16_t speedRR, uint16_t Acc_Lat, uint16_t rpm)
{
	float Slip_Ratio=0, Medium_Velocity=0, rpm_calc=0;
	
	if(ShiftUP_ON==0 && ShiftDN_ON==0)
	{
		if(speedFL>0 && speedFR>0 && speedRR>0 && speedRL>0)
		{
			Medium_Velocity= (float)((speedFR+speedFL)/(2.0f*1000.0f)); //m/s
			if(speedRR >= speedRL)
			{
				Slip_Ratio = (float)((((speedRR/1000.0f)-Medium_Velocity)/Medium_Velocity)*100.0f);
			}
			else
			{
				Slip_Ratio = (float)((((speedRL/1000.0f)-Medium_Velocity)/Medium_Velocity)*100.0f);
			}
			if(Slip_Ratio>0)
			{
				SR_Raw=(uint16_t)Slip_Ratio;
				rpm_calc=(float)rpm/60.0f;  //rpm to Hz
				rpm_calc=(10000.0f/rpm_calc)/2.0f;  //Hz to 0.1ms (at 12000 rpm, rpm_calc is 25, which means 2.5ms each combustion)
				TC_CUTV1(Slip_Ratio, SLIP_RATIO_TARGET,rpm_calc);
			}
		} 
	}
}



void TIM_CUT_Set(uint32_t tim_cnt, uint32_t tim_arr, TIM_HandleTypeDef *tim)
{
	tim->Instance->CNT = tim_cnt;
	tim->Instance->ARR = tim_arr;
	HAL_TIM_Base_Start_IT(tim);
}


void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim4);
	HAL_TIM_Base_Stop_IT(&htim4);
	SPARK_CUT_OFF;
	TC_CUT_ON=0;
	if(FTS_delay_ON)
	{
		TIM_CUT_Set(0,Gear_Cut_Time[1],&htim4);
		SPARK_CUT_ON;
		FTS_delay_ON=0;
	}
}


void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);
	HAL_TIM_Base_Stop_IT(&htim6);
	if(ShiftUP_ON)
	{
		SHIFTUP_OFF;
		ShiftUP_ON=0;
	}
	if(ShiftDN_ON)
	{
		SHIFTDN_OFF;
		ShiftDN_ON=0;
	}
}


void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(SHIFTUP_IN_Pin);	
	if(!ShiftUP_ON)
	{
		if(TC_CUT_ON)
		{
			HAL_TIM_Base_Stop_IT(&htim4);
			SPARK_CUT_OFF;
			TC_CUT_ON=0;
		}	
		if(GEAR==1 && FTS_cut_delay!=0)
		{
			ShiftUP_ON=1;
			shift_up_req=1;
			FTS_delay_ON=1;
			TIM_CUT_Set(0,FTS_cut_delay,&htim4);
			TIM_CUT_Set(0,Gear_Actuator_Time[GEAR],&htim6);
			SHIFTUP_ON;
		}
		else
		{
			ShiftUP_ON=1;
			shift_up_req=1;
			TIM_CUT_Set(0,Gear_Cut_Time[GEAR],&htim4);
			SPARK_CUT_ON;
			TIM_CUT_Set(0,Gear_Actuator_Time[GEAR],&htim6);
			SHIFTUP_ON;
		}
	}
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(SHIFTDN_IN_Pin);
	if(!ShiftDN_ON)
	{
		if(TC_CUT_ON)
		{
			HAL_TIM_Base_Stop_IT(&htim4);
			SPARK_CUT_OFF;
			TC_CUT_ON=0;
		}	
		ShiftDN_ON=1;
		shift_dn_req=1;
		TIM_CUT_Set(0,Gear_Actuator_Time[GEAR],&htim6);
		SHIFTDN_ON;
	}
}


void TC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO;
	
	RCC_SPARK_CUT_GPIO_CLK_ENABLE;
	
	GPIO.Pin = SPARK_CUT_Pin;
	GPIO.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO.Pull = GPIO_NOPULL;
  GPIO.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(SPARK_CUT_Port, &GPIO);
	
	SPARK_CUT_OFF;
}


void TC_TIM_Config(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 8400-1;  // 84MHz/8400=10KHz -> Tick=0.1ms
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 500;   // Overflows at 6.55s
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim4);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
}


void FTS_TIM_Config(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 8400-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0xFFFF;
  HAL_TIM_Base_Init(&htim6);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);
//	TIM_ClockConfigTypeDef sClockSourceConfig;
//  TIM_MasterConfigTypeDef sMasterConfig;

//  htim4.Instance = TIM6;
//  htim4.Init.Prescaler = 8400-1;  // 84MHz/8400=10KHz -> Tick=0.1ms
//  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim4.Init.Period = 0xFFFF-1;   // Overflows at 6.55s
//  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  HAL_TIM_Base_Init(&htim6);

//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  HAL_TIM_ConfigClockSource(&htim6, &sClockSourceConfig);

//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);
}


void FTS_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO;
	
	__GPIOC_CLK_ENABLE();
	
	GPIO.Pin = SHIFTUP_OUT_Pin | SHIFTDN_OUT_Pin;
  GPIO.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO.Pull = GPIO_NOPULL;
  GPIO.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(SHIFT_Port, &GPIO);
	
	HAL_GPIO_WritePin(SHIFT_Port,SHIFTUP_IN_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SHIFT_Port,SHIFTDN_IN_Pin,GPIO_PIN_RESET);
	
	GPIO.Pin = SHIFTUP_IN_Pin | SHIFTDN_IN_Pin;
  GPIO.Mode = GPIO_MODE_IT_FALLING;
  GPIO.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SHIFT_Port, &GPIO);
	
	HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	
	SHIFTDN_OFF;
	SHIFTUP_OFF;
}
