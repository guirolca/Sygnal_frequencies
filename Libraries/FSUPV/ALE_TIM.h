#ifndef ALE_TIM_H
#define ALE_TIM_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define __ALE_TIM_DISABLE(__TIMX__) \
                        do { \
                          if (((__TIMX__)->CCER & TIM_CCER_CCxE_MASK) == 0) \
                          { \
                            if(((__TIMX__)->CCER & TIM_CCER_CCxNE_MASK) == 0) \
                            { \
                              (__TIMX__)->CR1 &= ~(TIM_CR1_CEN); \
                            } \
                          } \
                        } while(0)

void TIMx_CLK_Enable(TIM_TypeDef *TIMx);
void TIMx_CLK_Disable(TIM_TypeDef *TIMx);
IRQn_Type TIM_Get_IRQn(TIM_TypeDef *TIMx);
void TIMx_Init(TIM_HandleTypeDef *htimx, TIM_TypeDef *TIMx, uint32_t preescaler, uint16_t period);
void ALE_TIM_SetPeriod(TIM_TypeDef *TIMx, uint32_t period);
void ALE_TIM_Stop_IT(TIM_TypeDef *TIMx);
void ALE_TIM_Start_IT(TIM_TypeDef *TIMx);												
#endif
