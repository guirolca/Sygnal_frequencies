#include "ALE_GPIO.h"

void ALE_GPIO_Clk_Enable(GPIO_TypeDef *GPIOx){
	if (GPIOx == GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}else if (GPIOx == GPIOB){
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}else if (GPIOx == GPIOC){
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}else if (GPIOx == GPIOD){
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}else if (GPIOx == GPIOE){
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}else if (GPIOx == GPIOF){
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}else if (GPIOx == GPIOG){
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}else if (GPIOx == GPIOH){
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
	#ifdef GPIO_IJK
	else if (GPIOx == GPIOI){
		__HAL_RCC_GPIOI_CLK_ENABLE();
	}else if (GPIOx == GPIOJ){
		__HAL_RCC_GPIOJ_CLK_ENABLE();
	}else if (GPIOx == GPIOK){
		__HAL_RCC_GPIOK_CLK_ENABLE();
	}
	#endif
}	

void ALE_GPIO_Clk_Disable(GPIO_TypeDef *GPIOx){
	if (GPIOx == GPIOA){
		__HAL_RCC_GPIOA_CLK_DISABLE();
	}else if (GPIOx == GPIOB){
		__HAL_RCC_GPIOB_CLK_DISABLE();
	}else if (GPIOx == GPIOC){
		__HAL_RCC_GPIOC_CLK_DISABLE();
	}else if (GPIOx == GPIOD){
		__HAL_RCC_GPIOD_CLK_DISABLE();
	}else if (GPIOx == GPIOE){
		__HAL_RCC_GPIOE_CLK_DISABLE();
	}else if (GPIOx == GPIOF){
		__HAL_RCC_GPIOF_CLK_DISABLE();
	}else if (GPIOx == GPIOG){
		__HAL_RCC_GPIOG_CLK_DISABLE();
	}else if (GPIOx == GPIOH){
		__HAL_RCC_GPIOH_CLK_DISABLE();
	}
	#ifdef GPIO_IJK
	else if (GPIOx == GPIOI){
		__HAL_RCC_GPIOI_CLK_DISABLE();
	}else if (GPIOx == GPIOJ){
		__HAL_RCC_GPIOJ_CLK_DISABLE();
	}else if (GPIOx == GPIOK){
		__HAL_RCC_GPIOK_CLK_DISABLE();
	}
	#endif
}	
