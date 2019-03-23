#ifndef ALE_GPIO_F3_H
#define ALE_GPIO_F3_H


#include "stm32f3xx_hal.h"

void ALE_GPIO_Clk_Enable(GPIO_TypeDef *GPIOx);
void ALE_GPIO_Clk_Disable(GPIO_TypeDef *GPIOx);

#endif
