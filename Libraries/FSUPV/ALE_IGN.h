#ifndef ALE_IGN_H
#define ALE_IGN_H
#include "stm32f4xx_hal.h"

typedef struct {
	GPIO_TypeDef *GPIOx_out;
	uint32_t GPIO_PIN_x_out;
	GPIO_TypeDef *GPIOx_in;
	uint32_t GPIO_PIN_x_in;
	
}ALE_IGN_Channel_t;

typedef struct {
	ALE_IGN_Channel_t channels[4];
	TIM_HandleTypeDef htimup;
	TIM_HandleTypeDef htimdn;
}ALE_IGN_t;	

typedef struct {
	ALE_IGN_Channel_t channels[2];
	TIM_HandleTypeDef htim;
}ALE_SHIFT_t;	





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
	TIM_TypeDef *TIM_counter);

void ALE_SHIFT_Init(		
	GPIO_TypeDef *shiftup_in_GPIOx, uint32_t shiftup_in_pin,
	GPIO_TypeDef *shiftdn_in_GPIOx, uint32_t shiftdn_in_pin,
	GPIO_TypeDef *shiftup_out_GPIOx, uint32_t shiftup_out_pin,
	GPIO_TypeDef *shiftdn_out_GPIOx, uint32_t shiftdn_out_pin,
	TIM_TypeDef *TIM_sh, TIM_TypeDef *TIM_cut);


void ALE_IGN_set_delay(float delaydeg);
void ALE_IGN_shift_up	(void);
void ALE_IGN_shift_dn	(void);
void ALE_IGN_IN_event(uint8_t channel);
void ALE_IGN_TIMccounter_Handler(void);
void ALE_IGN_TIMup_Handler(void);
void ALE_IGN_TIMdn_Handler(void);
void ALE_IGN_TIMcut_Handler(void);
void ALE_IGN_TIMsh_Handler(void);
void ALE_IGN_shift_noshift(float noshifttime);
void ALE_IGN_set_cutdelay(float delaydeg);
void ALE_IGN_shift_up_timing(float cut, float delaytime, float shifttime);
void ALE_IGN_shift_dn_timing(float cut, float delaytime, float shifttime);


#endif
