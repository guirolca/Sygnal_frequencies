//#include "defines.h"
#include "ALE_IGN.h"
#include "ALE_GPIO_F4.h"
#include "mxconstants.h"
#include "tm_stm32_exti.h"
#define MAXEVENTS 10
#ifndef MAXDELAY_DEG
#define MAXDELAY_DEG 90
#endif

#define IGN_R(_x_) HAL_GPIO_ReadPin(ign.channels[_x_].GPIOx_in,ign.channels[_x_].GPIO_PIN_x_in)
#define IGN_H(_x_) HAL_GPIO_WritePin(ign.channels[_x_].GPIOx_out,ign.channels[_x_].GPIO_PIN_x_out,GPIO_PIN_SET)
#define IGN_L(_x_) HAL_GPIO_WritePin(ign.channels[_x_].GPIOx_out,ign.channels[_x_].GPIO_PIN_x_out,GPIO_PIN_RESET)

#define SHFT_H(_x_) HAL_GPIO_WritePin(sht.channels[_x_].GPIOx_out,sht.channels[_x_].GPIO_PIN_x_out,GPIO_PIN_SET)
#define SHFT_L(_x_) HAL_GPIO_WritePin(sht.channels[_x_].GPIOx_out,sht.channels[_x_].GPIO_PIN_x_out,GPIO_PIN_RESET)

typedef enum{
	NONE=0,
	DELAY_UP,
	DELAY_DN,
	SHIFT_UP,
	SHIFT_DN,
	NO_SHIFT,
}TIMSH_STATE_t;

void ch1_h(void);
void ch2_h(void);
void ch3_h(void);
void ch4_h(void);
void ch1_l(void);
void ch2_l(void);
void ch3_l(void);
void ch4_l(void);
void nada(void);

ALE_IGN_t ign;
ALE_SHIFT_t sht;

void ch1_h(void){
	IGN_H(0);
}
void ch2_h(void){
	IGN_H(1);
}
void ch3_h(void){
	IGN_H(2);
}
void ch4_h(void){
	IGN_H(3);
}
void ch1_l(void){
	IGN_L(0);
}
void ch2_l(void){
	IGN_L(1);
}
void ch3_l(void){
	IGN_L(2);
}
void ch4_l(void){
	IGN_L(3);
}
void nada(void){
}


TIM_TypeDef *TIMup;
TIM_TypeDef *TIMdn;
TIM_TypeDef *TIMsh;
TIM_TypeDef *TIMcut;
TIM_TypeDef *TIMcounter;

TIM_HandleTypeDef htimup;
TIM_HandleTypeDef htimdn;
TIM_HandleTypeDef htimsh;
TIM_HandleTypeDef htimcut;
TIM_HandleTypeDef htimcnt;

uint16_t timcountH=0;
uint32_t ign_delay, cuttime_up=500, delaysh_up=0, shifttime_up=500, cuttime_dn=0, delaysh_dn=0, shifttime_dn=500, noshift_time=100;
TIMSH_STATE_t timsh_state=NONE;
float ign_deg=0, ign_cutdeg=50.0f;
uint32_t lasttimcount[8]={0,0,0,0,0,0,0,0};
uint8_t fullcut=0, shiftcut=0;
void (*next_up)(void);
void (*next_dn)(void);

void ALE_NVIC_TIM_Enable(TIM_TypeDef *TIMx);
void ALE_NVIC_TIM_Disable(TIM_TypeDef *TIMx);
void ALE_IGN_Update_delay(uint8_t channel);
void ALE_IGN_ResetTimer(TIM_TypeDef *TIMx, uint32_t value);

void ALE_NVIC_TIM_Enable(TIM_TypeDef *TIMx){
	#ifdef TIM1
	if (TIMx==TIM1){
		__TIM1_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	}
	#endif
	#ifdef TIM2
	if (TIMx==TIM2){
		__TIM2_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
	#endif
	#ifdef TIM3
	if (TIMx==TIM3){
		__TIM3_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}
	#endif
	#ifdef TIM4
	if (TIMx==TIM4){
		__TIM4_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}
	#endif
	#ifdef TIM5
	if (TIMx==TIM5){
		__TIM5_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}
	#endif
	#ifdef TIM6
	if (TIMx==TIM6){
		__TIM6_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
	#endif
	#ifdef TIM7
	if (TIMx==TIM7){
		__TIM7_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}
	#endif
	#ifdef TIM8
	if (TIMx==TIM8){
		__TIM8_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
	}
	#endif
	#ifdef TIM9
	if (TIMx==TIM9){
		__TIM9_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	}
	#endif
	#ifdef TIM10
	if (TIMx==TIM10){
		__TIM10_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	}
	#endif
	#ifdef TIM11
	if (TIMx==TIM11){
		__TIM11_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
	}
	#endif
	#ifdef TIM12
	if (TIMx==TIM12){
		__TIM12_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
	}
	#endif
	#ifdef TIM13
	if (TIMx==TIM13){
		__TIM13_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
	}
	#endif
	#ifdef TIM14
	if (TIMx==TIM14){
		__TIM14_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
	}
	#endif
}

void ALE_NVIC_TIM_Disable(TIM_TypeDef *TIMx){
	#ifdef TIM1
	if (TIMx==TIM1){
		__TIM1_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	}
	#endif
	#ifdef TIM2
	if (TIMx==TIM2){
		__TIM2_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
	}
	#endif
	#ifdef TIM3
	if (TIMx==TIM3){
		__TIM3_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
	}
	#endif
	#ifdef TIM4
	if (TIMx==TIM4){
		__TIM4_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
	}
	#endif
	#ifdef TIM5
	if (TIMx==TIM5){
		__TIM5_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM5_IRQn);
	}
	#endif
	#ifdef TIM6
	if (TIMx==TIM6){
		__TIM6_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
	#endif
	#ifdef TIM7
	if (TIMx==TIM7){
		__TIM7_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
	}
	#endif
	#ifdef TIM8
	if (TIMx==TIM8){
		__TIM8_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
	}
	#endif
	#ifdef TIM9
	if (TIMx==TIM9){
		__TIM9_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
	}
	#endif
	#ifdef TIM10
	if (TIMx==TIM10){
		__TIM10_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	}
	#endif
	#ifdef TIM11
	if (TIMx==TIM11){
		__TIM11_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
	}
	#endif
	#ifdef TIM12
	if (TIMx==TIM12){
		__TIM12_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);
	}
	#endif
	#ifdef TIM13
	if (TIMx==TIM13){
		__TIM13_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
	}
	#endif
	#ifdef TIM14
	if (TIMx==TIM14){
		__TIM14_CLK_DISABLE();
    //HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0, 0);
    HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
	}
	#endif
}


void ALE_IGN_set_delay(float delaydeg){
	ign_deg=delaydeg;
}

void ALE_IGN_set_cutdelay(float delaydeg){
	ign_cutdeg=delaydeg;
}

void ALE_IGN_shift_up_timing(float cut, float delaytime, float shifttime){
	cuttime_up = (uint32_t)(cut*10);
	delaysh_up = (uint32_t)(delaytime*10);
	shifttime_up = (uint32_t)(shifttime*10);
}

void ALE_IGN_shift_dn_timing(float cut, float delaytime, float shifttime){
	cuttime_dn = (uint32_t)(cut*10);
	delaysh_dn = (uint32_t)(delaytime*10);
	shifttime_dn = (uint32_t)(shifttime*10);
}

void ALE_IGN_shift_noshift(float noshifttime){
	noshift_time=(uint32_t)(noshifttime*10);
}
	
void ALE_IGN_init(
	GPIO_TypeDef *ign1_in_GPIOx, uint32_t ign1_in_pin,
	GPIO_TypeDef *ign2_in_GPIOx, uint32_t ign2_in_pin,
	GPIO_TypeDef *ign3_in_GPIOx, uint32_t ign3_in_pin,
	GPIO_TypeDef *ign4_in_GPIOx, uint32_t ign4_in_pin,
	GPIO_TypeDef *ign1_out_GPIOx, uint32_t ign1_out_pin,
	GPIO_TypeDef *ign2_out_GPIOx, uint32_t ign2_out_pin,
	GPIO_TypeDef *ign3_out_GPIOx, uint32_t ign3_out_pin,
	GPIO_TypeDef *ign4_out_GPIOx, uint32_t ign4_out_pin,
	TIM_TypeDef *TIM_up, TIM_TypeDef *TIM_dn, 
	TIM_TypeDef *TIM_counter)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	ALE_GPIO_Clk_Enable(ign1_out_GPIOx);
	ALE_GPIO_Clk_Enable(ign2_out_GPIOx);
	ALE_GPIO_Clk_Enable(ign3_out_GPIOx);
	ALE_GPIO_Clk_Enable(ign4_out_GPIOx);
	
	
	ign.channels[0].GPIOx_in=ign1_in_GPIOx;
	ign.channels[1].GPIOx_in=ign2_in_GPIOx;
	ign.channels[2].GPIOx_in=ign3_in_GPIOx;
	ign.channels[3].GPIOx_in=ign4_in_GPIOx;
	ign.channels[0].GPIO_PIN_x_in = ign1_in_pin;
	ign.channels[1].GPIO_PIN_x_in = ign2_in_pin;
	ign.channels[2].GPIO_PIN_x_in = ign3_in_pin;
	ign.channels[3].GPIO_PIN_x_in = ign4_in_pin;
	
	ign.channels[0].GPIOx_out=ign1_out_GPIOx;
	ign.channels[1].GPIOx_out=ign2_out_GPIOx;
	ign.channels[2].GPIOx_out=ign3_out_GPIOx;
	ign.channels[3].GPIOx_out=ign4_out_GPIOx;
	ign.channels[0].GPIO_PIN_x_out = ign1_out_pin;
	ign.channels[1].GPIO_PIN_x_out = ign2_out_pin;
	ign.channels[2].GPIO_PIN_x_out = ign3_out_pin;
	ign.channels[3].GPIO_PIN_x_out = ign4_out_pin;
	
	TM_EXTI_Attach(ign1_in_GPIOx,ign1_in_pin,TM_EXTI_Trigger_Rising_Falling);
	TM_EXTI_Attach(ign2_in_GPIOx,ign2_in_pin,TM_EXTI_Trigger_Rising_Falling);
	TM_EXTI_Attach(ign3_in_GPIOx,ign3_in_pin,TM_EXTI_Trigger_Rising_Falling);
	TM_EXTI_Attach(ign4_in_GPIOx,ign4_in_pin,TM_EXTI_Trigger_Rising_Falling);
	
	TIMup = TIM_up;
	TIMdn = TIM_dn;
	TIMcounter=TIM_counter;
	ALE_NVIC_TIM_Enable(TIMup);
	htimup.Instance = TIMup;
  htimup.Init.Prescaler = 84-1; //1us
  htimup.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimup.Init.Period = 0xFFFF;  
  htimup.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimup);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimup, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimup, &sMasterConfig);
	
	ALE_NVIC_TIM_Enable(TIMdn);
	htimdn.Instance = TIMdn;
  htimdn.Init.Prescaler = 84-1; //1us
  htimdn.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimdn.Init.Period = 0xFFFF; 
  htimdn.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimdn);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimdn, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimdn, &sMasterConfig);
	
	ALE_NVIC_TIM_Enable(TIMcounter);
	htimcnt.Instance = TIMcounter;
  htimcnt.Init.Prescaler = 84-1; //1us
  htimcnt.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimcnt.Init.Period = 0xFFFF;  
  htimcnt.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimcnt);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimcnt, &sClockSourceConfig);

	HAL_TIM_Base_Start_IT(&htimcnt);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimcnt, &sMasterConfig);
	HAL_TIM_Base_Start(&htimcnt);
	GPIO_InitStruct.Pin = ign1_out_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(ign1_in_GPIOx, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = ign2_out_pin;
	HAL_GPIO_Init(ign2_in_GPIOx, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = ign3_out_pin;
	HAL_GPIO_Init(ign3_in_GPIOx, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = ign4_out_pin;
	HAL_GPIO_Init(ign4_in_GPIOx, &GPIO_InitStruct);
	
	
}


void ALE_SHIFT_Init(		
	GPIO_TypeDef *shiftup_in_GPIOx, uint32_t shiftup_in_pin,
	GPIO_TypeDef *shiftdn_in_GPIOx, uint32_t shiftdn_in_pin,
	GPIO_TypeDef *shiftup_out_GPIOx, uint32_t shiftup_out_pin,
	GPIO_TypeDef *shiftdn_out_GPIOx, uint32_t shiftdn_out_pin,
	TIM_TypeDef *TIM_sh, TIM_TypeDef *TIM_cut)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	ALE_GPIO_Clk_Enable(shiftup_out_GPIOx);
	ALE_GPIO_Clk_Enable(shiftdn_out_GPIOx);

	sht.channels[0].GPIOx_in=shiftup_in_GPIOx;
	sht.channels[1].GPIOx_in=shiftdn_in_GPIOx;
	sht.channels[0].GPIO_PIN_x_in = shiftup_in_pin;
	sht.channels[1].GPIO_PIN_x_in = shiftdn_in_pin;
	
	sht.channels[0].GPIOx_out=shiftup_out_GPIOx;
	sht.channels[1].GPIOx_out=shiftdn_out_GPIOx;
	sht.channels[0].GPIO_PIN_x_out = shiftup_out_pin;
	sht.channels[1].GPIO_PIN_x_out = shiftdn_out_pin;
	
	TM_EXTI_Attach(shiftup_in_GPIOx,shiftup_in_pin,TM_EXTI_Trigger_Rising);
	TM_EXTI_Attach(shiftdn_in_GPIOx,shiftdn_in_pin,TM_EXTI_Trigger_Rising);
	
	TIMsh = TIM_sh;
	TIMcut = TIM_cut;
	
	ALE_NVIC_TIM_Enable(TIMsh);
	htimsh.Instance = TIMsh;
  htimsh.Init.Prescaler = 8400-1;
  htimsh.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimsh.Init.Period = 0xFFFF; //10000 1seg
  htimsh.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimsh);
	
	 sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimsh, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimsh, &sMasterConfig);
	
	ALE_NVIC_TIM_Enable(TIMcut);
	htimcut.Instance = TIMcut;
  htimcut.Init.Prescaler = 8400-1;
  htimcut.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimcut.Init.Period = 0xFFFF; //10000 1seg
  htimcut.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimcut);
	
	 sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htimcut, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimcut, &sMasterConfig);
	
	GPIO_InitStruct.Pin = shiftup_out_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(shiftup_out_GPIOx, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = shiftdn_out_pin;
	HAL_GPIO_Init(shiftdn_out_GPIOx, &GPIO_InitStruct);
}

void ALE_IGN_shift_up	(void){
	timsh_state=DELAY_UP;
	if (timsh_state==NONE){
		if (cuttime_up){
			ALE_IGN_ResetTimer(TIMcut,cuttime_up);
			shiftcut=1;
		}
		
		ALE_IGN_ResetTimer(TIMsh,delaysh_up);
		
	}
}
	

void ALE_IGN_shift_dn	(void){
	timsh_state=DELAY_DN;
	if (timsh_state==NONE){
		if (cuttime_dn){
			ALE_IGN_ResetTimer(TIMcut,cuttime_dn);
			shiftcut=1;
			
		}
		
		ALE_IGN_ResetTimer(TIMsh,delaysh_dn);
	}
}		

void ALE_IGN_Update_delay(uint8_t channel){
	uint32_t period,counter;
	float delayf;
	float degc;
	counter=TIMcounter->CNT;
	counter += ((uint32_t)(timcountH)<<16);
	if (counter<lasttimcount[channel]){
		period=0xFFFFFFFF-(lasttimcount[channel]-counter);
		period++;
	}else{
		period=counter-lasttimcount[channel];
	}
	lasttimcount[channel]=counter;
	if (shiftcut){
		degc=ign_deg+ign_cutdeg;
	}else{
		degc=ign_deg;
	}
	if (degc>MAXDELAY_DEG){
		fullcut=1;
	}else{
		delayf=(degc*period)/720.0f;
		ign_delay = (uint32_t)delayf;
		fullcut=0;
	}

}

void ALE_IGN_ResetTimer(TIM_TypeDef *TIMx, uint32_t value){
	TIMx->CR1 &= ~TIM_CR1_CEN;
	TIMx->DIER &= ~TIM_DIER_UIE;
	TIMx->CNT=0;
	TIMx->ARR=value;
	TIMx->SR &= ~TIM_SR_UIF;
	TIMx->DIER |= TIM_DIER_UIE;
	
	TIMx->CR1 |= TIM_CR1_CEN;
}

void ALE_IGN_IN_event(uint8_t channel){

	switch(channel){
		case 1:
			if (IGN_R(0)){
				ALE_IGN_Update_delay(0);
				if (!fullcut){
					if (ign_delay){
						next_up = ch1_h;
						ALE_IGN_ResetTimer(TIMup,ign_delay);						
					}else{
						IGN_H(0);
					}
				}
				
			}else{
				ALE_IGN_Update_delay(4);
				if (!fullcut){
					if (ign_delay){
						next_dn = ch1_l;
						ALE_IGN_ResetTimer(TIMdn,ign_delay);
					}else{
						IGN_L(0);
					}
				}	
			}
			break;
		case 2:
			if (IGN_R(1)){
				ALE_IGN_Update_delay(1);
				if (!fullcut){
					if (ign_delay){
						next_up = ch2_h;
						ALE_IGN_ResetTimer(TIMup,ign_delay);
					}else{
						IGN_H(1);
					}
				}
			}else{
				ALE_IGN_Update_delay(5);
				if (!fullcut){
					if (ign_delay){
						next_dn = ch2_l;
						ALE_IGN_ResetTimer(TIMdn,ign_delay);
					}else{
						IGN_L(1);
					}
				}
			}
			break;
		case 3:
			if (IGN_R(2)){
				ALE_IGN_Update_delay(2);
				if (!fullcut){
					if (ign_delay){
						next_up = ch3_h;
						ALE_IGN_ResetTimer(TIMup,ign_delay);
					}else{
						IGN_H(2);
					}
				}
				
			}else{
				ALE_IGN_Update_delay(6);
				if (!fullcut){
					if (ign_delay){
						next_dn = ch3_l;
						ALE_IGN_ResetTimer(TIMdn,ign_delay);
					}else{
						IGN_L(2);
					}
				}
			}
			break;
		case 4:
			if (IGN_R(3)){
				ALE_IGN_Update_delay(3);
				if (!fullcut){
					if (ign_delay){
						next_up = ch4_h;
						ALE_IGN_ResetTimer(TIMup,ign_delay);
					}else{
						IGN_H(3);
					}
				}
				
			}else{
				ALE_IGN_Update_delay(7);
				if (!fullcut){
					if (ign_delay){
						
						next_dn = ch4_l;
						ALE_IGN_ResetTimer(TIMdn,ign_delay);
						
					}else{
						IGN_L(3);
					}
				}
			}
			break;
	}
}

void ALE_IGN_TIMccounter_Handler(void){
	if (((TIMcounter->SR & TIM_SR_UIF) == TIM_SR_UIF) && ((TIMcounter->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)){
		timcountH++;
		TIMcounter->SR &= ~TIM_SR_UIF;
	}
		
}
	
void ALE_IGN_TIMup_Handler(void){
	if (((TIMup->SR & TIM_SR_UIF) == TIM_SR_UIF) && ((TIMup->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)){
		TIMup->SR &= ~TIM_SR_UIF;
		TIMup->DIER &= ~TIM_DIER_UIE;
		TIMup->CR1 &= ~TIM_CR1_CEN;
		TIMup->CNT=0;
		next_up();
	}
}
void ALE_IGN_TIMdn_Handler(void){
	if (((TIMdn->SR & TIM_SR_UIF) == TIM_SR_UIF) && ((TIMdn->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)){
		TIMdn->SR &= ~TIM_SR_UIF;
		TIMdn->DIER &= ~TIM_DIER_UIE;
		TIMdn->CR1 &= ~TIM_CR1_CEN;
		TIMdn->CNT=0;
		next_dn();
	}
}

void ALE_IGN_TIMcut_Handler(void){
	if (((TIMcut->SR & TIM_SR_UIF) == TIM_SR_UIF) && ((TIMcut->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)){
		TIMcut->SR &= ~TIM_SR_UIF;
		TIMcut->DIER &= ~TIM_DIER_UIE;
		TIMcut->CR1 &= ~TIM_CR1_CEN;
		TIMcut->CNT=0;
		shiftcut=0;
	}
}
void ALE_IGN_TIMsh_Handler(void){
	if (((TIMsh->SR & TIM_SR_UIF) == TIM_SR_UIF) && ((TIMsh->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)){
		TIMsh->SR &= ~TIM_SR_UIF;
		switch(timsh_state){
			case DELAY_UP:
				ALE_IGN_ResetTimer(TIMsh,shifttime_up);
				SHFT_H(0);
				timsh_state = SHIFT_UP;
				
				
			break;
			case SHIFT_UP:
				ALE_IGN_ResetTimer(TIMsh,noshift_time);
				SHFT_L(0);
				timsh_state = NO_SHIFT;
			break;
			case DELAY_DN:
				ALE_IGN_ResetTimer(TIMsh,shifttime_dn);
				SHFT_H(1);
				timsh_state = SHIFT_DN;
			break;
			case SHIFT_DN:
				ALE_IGN_ResetTimer(TIMsh,noshift_time);
				SHFT_L(1);
				timsh_state = NO_SHIFT;
			break;
			case NO_SHIFT:
			TIMsh->DIER &= ~TIM_DIER_UIE;	
			TIMsh->CR1 &= ~TIM_CR1_CEN;
				TIMsh->CNT=0;
				
				timsh_state=NONE;
			default:
				break;
		}
	}

}

	
	
	


