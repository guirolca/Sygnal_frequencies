#ifndef TCS_H
#define TCS_H

#include <stdint.h>

#define SLIP_RATIO_TARGET 15
#define COMBUSTION_G 3//(5/10)

#define SPARK_CUT_Pin GPIO_PIN_8
#define SPARK_CUT_Port GPIOC
#define SHIFTUP_IN_Pin GPIO_PIN_1
#define SHIFTDN_IN_Pin GPIO_PIN_2
#define SHIFTUP_OUT_Pin GPIO_PIN_3
#define SHIFTDN_OUT_Pin GPIO_PIN_4
#define SHIFT_Port GPIOC

#define RCC_SPARK_CUT_GPIO_CLK_ENABLE  __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPARK_CUT_ON HAL_GPIO_WritePin(SPARK_CUT_Port,SPARK_CUT_Pin,GPIO_PIN_RESET)
#define SPARK_CUT_OFF HAL_GPIO_WritePin(SPARK_CUT_Port,SPARK_CUT_Pin,GPIO_PIN_SET)
#define SHIFTUP_ON HAL_GPIO_WritePin(SHIFT_Port,SHIFTUP_OUT_Pin,GPIO_PIN_SET)
#define SHIFTUP_OFF HAL_GPIO_WritePin(SHIFT_Port,SHIFTUP_OUT_Pin,GPIO_PIN_RESET)
#define SHIFTDN_ON HAL_GPIO_WritePin(SHIFT_Port,SHIFTDN_OUT_Pin,GPIO_PIN_SET)
#define SHIFTDN_OFF HAL_GPIO_WritePin(SHIFT_Port,SHIFTDN_OUT_Pin,GPIO_PIN_RESET)


void TC_CUTV1(float SR, uint8_t SR_target,float rpm_ms);
void TIM_CUT_Set(uint32_t tim_cnt, uint32_t tim_arr, TIM_HandleTypeDef *tim);
void FTS_TIM_Config(void);
void FTS_GPIO_Config(void);
void TC_GPIO_Config(void);
void TC_TIM_Config(void);
void TC_ON(uint16_t speedFL, uint16_t speedFR, uint16_t speedRL, uint16_t speedRR, uint16_t Acc_Lat, uint16_t rpm);
void TIM4_IRQHandler(void);

#endif
