#ifndef ALE_GPIO_F1_H
#define ALE_GPIO_F1_H


#include "stm32f1xx_hal.h"

void ALE_GPIO_Clk_Enable(GPIO_TypeDef *GPIOx);
void ALE_GPIO_Clk_Disable(GPIO_TypeDef *GPIOx);

#endif
