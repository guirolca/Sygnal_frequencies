#ifndef ALE_GPIO_F4_H
#define ALE_GPIO_F4_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"


void ALE_GPIO_Clk_Enable(GPIO_TypeDef *GPIOx);
void ALE_GPIO_Clk_Disable(GPIO_TypeDef *GPIOx);

#endif
