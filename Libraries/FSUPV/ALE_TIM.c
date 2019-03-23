#include "ALE_Tim.h"

void TIMx_CLK_Enable(TIM_TypeDef *TIMx){
	#ifdef TIM1
	if (TIMx == TIM1){
		__TIM1_CLK_ENABLE();
	}
	#endif
	#ifdef TIM2
	if (TIMx == TIM2){
		__TIM2_CLK_ENABLE();
	}
	#endif
	#ifdef TIM3
	if (TIMx == TIM3){
		__TIM3_CLK_ENABLE();
	}
	#endif
	#ifdef TIM4
	if (TIMx == TIM4){
		__TIM4_CLK_ENABLE();
	}
	#endif
	#ifdef TIM5
	if (TIMx == TIM5){
		__TIM5_CLK_ENABLE();
	}
	#endif
	#ifdef TIM6
	if (TIMx == TIM6){
		__TIM6_CLK_ENABLE();
	}
	#endif
	#ifdef TIM7
	if (TIMx == TIM7){
		__TIM7_CLK_ENABLE();
	}
	#endif
	#ifdef TIM8
	if (TIMx == TIM8){
		__TIM8_CLK_ENABLE();
	}
	#endif
	#ifdef TIM9
	if (TIMx == TIM9){
		__TIM9_CLK_ENABLE();
	}
	#endif
	#ifdef TIM10
	if (TIMx == TIM10){
		__TIM10_CLK_ENABLE();
	}
	#endif
	#ifdef TIM11
	if (TIMx == TIM11){
		__TIM11_CLK_ENABLE();
	}
	#endif
	#ifdef TIM12
	if (TIMx == TIM12){
		__TIM12_CLK_ENABLE();
	}
	#endif
	#ifdef TIM13
	if (TIMx == TIM13){
		__TIM13_CLK_ENABLE();
	}
	#endif
	#ifdef TIM14
	if (TIMx == TIM14){
		__TIM14_CLK_ENABLE();
	}
	#endif
	#ifdef TIM15
	if (TIMx == TIM15){
		__TIM15_CLK_ENABLE();
	}
	#endif
	#ifdef TIM16
	if (TIMx == TIM16){
		__TIM16_CLK_ENABLE();
	}
	#endif
	#ifdef TIM17
	if (TIMx == TIM17){
		__TIM17_CLK_ENABLE();
	}
	#endif
}


void TIMx_CLK_Disable(TIM_TypeDef *TIMx){
	#ifdef TIM1
	if (TIMx == TIM1){
		__TIM1_CLK_DISABLE();
	}
	#endif
	#ifdef TIM2
	if (TIMx == TIM2){
		__TIM2_CLK_DISABLE();
	}
	#endif
	#ifdef TIM3
	if (TIMx == TIM3){
		__TIM3_CLK_DISABLE();
	}
	#endif
	#ifdef TIM4
	if (TIMx == TIM4){
		__TIM4_CLK_DISABLE();
	}
	#endif
	#ifdef TIM5
	if (TIMx == TIM5){
		__TIM5_CLK_DISABLE();
	}
	#endif
	#ifdef TIM6
	if (TIMx == TIM6){
		__TIM6_CLK_DISABLE();
	}
	#endif
	#ifdef TIM7
	if (TIMx == TIM7){
		__TIM7_CLK_DISABLE();
	}
	#endif
	#ifdef TIM8
	if (TIMx == TIM8){
		__TIM8_CLK_DISABLE();
	}
	#endif
	#ifdef TIM9
	if (TIMx == TIM9){
		__TIM9_CLK_DISABLE();
	}
	#endif
	#ifdef TIM10
	if (TIMx == TIM10){
		__TIM10_CLK_DISABLE();
	}
	#endif
	#ifdef TIM11
	if (TIMx == TIM11){
		__TIM11_CLK_DISABLE();
	}
	#endif
	#ifdef TIM12
	if (TIMx == TIM12){
		__TIM12_CLK_DISABLE();
	}
	#endif
	#ifdef TIM13
	if (TIMx == TIM13){
		__TIM13_CLK_DISABLE();
	}
	#endif
	#ifdef TIM14
	if (TIMx == TIM14){
		__TIM14_CLK_DISABLE();
	}
	#endif
	#ifdef TIM15
	if (TIMx == TIM15){
		__TIM15_CLK_DISABLE();
	}
	#endif
	#ifdef TIM16
	if (TIMx == TIM16){
		__TIM16_CLK_DISABLE();
	}
	#endif
	#ifdef TIM17
	if (TIMx == TIM17){
		__TIM17_CLK_DISABLE();
	}
	#endif
}
IRQn_Type TIM_Get_IRQn(TIM_TypeDef *TIMx){
	#ifdef TIM1
	if (TIMx==TIM1){
		return TIM1_UP_TIM10_IRQn;
	}
	#endif
	#ifdef TIM2
	if (TIMx==TIM2){
		return TIM2_IRQn;
	}
	#endif
	#ifdef TIM3
	if (TIMx==TIM3){
		return TIM3_IRQn;
	}
	#endif
	#ifdef TIM4
	if (TIMx==TIM4){
		return TIM4_IRQn;
	}
	#endif	
	#ifdef TIM5
	if (TIMx==TIM5){
		return TIM5_IRQn;
	}
	#endif
	#ifdef TIM6
	if (TIMx==TIM6){
		return TIM6_DAC_IRQn;
	}
	#endif
	#ifdef TIM7
	if (TIMx==TIM7){
		return TIM7_IRQn;
	}
	#endif
	#ifdef TIM8
	if (TIMx==TIM8){
		return TIM8_UP_TIM13_IRQn;
	}
	#endif
	#ifdef TIM9
	if (TIMx==TIM9){
		return TIM1_BRK_TIM9_IRQn;
	}
	#endif
	#ifdef TIM10
	if (TIMx==TIM10){
		return TIM1_UP_TIM10_IRQn;
	}
	#endif
	#ifdef TIM11
	if (TIMx==TIM11){
		return TIM1_TRG_COM_TIM11_IRQn;
	}
	#endif
	#ifdef TIM12
	if (TIMx==TIM12){
		return TIM8_BRK_TIM12_IRQn;
	}
	#endif
	#ifdef TIM13
	if (TIMx==TIM13){
		return TIM8_UP_TIM13_IRQn;
	}
	#endif
	#ifdef TIM14
	if (TIMx==TIM14){
		return TIM8_TRG_COM_TIM14_IRQn;
	}
	#endif
	return (IRQn_Type)0;
}

void TIMx_Init(TIM_HandleTypeDef *htimx, TIM_TypeDef *TIMx, uint32_t preescaler, uint16_t period)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
	
	TIMx_CLK_Enable(TIMx);
    HAL_NVIC_SetPriority(TIM_Get_IRQn(TIMx), 5, 0);
    HAL_NVIC_EnableIRQ(TIM_Get_IRQn(TIMx));
	
  htimx->Instance = TIMx;
  htimx->Init.Prescaler = preescaler;
  htimx->Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx->Init.Period = period; //1ms
  htimx->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(htimx);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(htimx, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(htimx, &sMasterConfig);
}

void ALE_TIM_Stop_IT(TIM_TypeDef *TIMx){
	TIMx->DIER &= ~TIM_DIER_UIE;
	__ALE_TIM_DISABLE(TIMx);
}

void ALE_TIM_Start_IT(TIM_TypeDef *TIMx){
	TIMx->DIER |= TIM_DIER_UIE;
	TIMx->CR1 |= TIM_CR1_CEN;
}

void ALE_TIM_SetPeriod(TIM_TypeDef *TIMx, uint32_t period){
	ALE_TIM_Stop_IT(TIMx);
	TIMx->CNT = 0;
	TIMx->ARR = period; //70ms cut + shift up
	TIMx->SR &= ~TIM_SR_UIF; 

	
}
