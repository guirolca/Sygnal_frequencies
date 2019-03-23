#include "ALE_GPIO_F4.h"

void ALE_GPIO_Clk_Enable(GPIO_TypeDef *GPIOx){
	#ifdef GPIOA
	if (GPIOx == GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
	}
	#endif
	#ifdef GPIOB
	if (GPIOx == GPIOB){
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOC
	 if (GPIOx == GPIOC){
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOD
	 if (GPIOx == GPIOD){
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOE
	 if (GPIOx == GPIOE){
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOF
	 if (GPIOx == GPIOF){
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOG
	 if (GPIOx == GPIOG){
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOH
	 if (GPIOx == GPIOH){
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOI
	 if (GPIOx == GPIOI){
		__HAL_RCC_GPIOI_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOJ
	 if (GPIOx == GPIOJ){
		__HAL_RCC_GPIOJ_CLK_ENABLE();
	}
	#endif
	#ifdef GPIOK
	 if (GPIOx == GPIOK){
		__HAL_RCC_GPIOK_CLK_ENABLE();
	}
	#endif
}	

void ALE_GPIO_Clk_Disable(GPIO_TypeDef *GPIOx){
	

	#ifdef GPIOA
	if (GPIOx == GPIOA){
		__HAL_RCC_GPIOA_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOB
	 if (GPIOx == GPIOB){
		__HAL_RCC_GPIOB_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOC
	 if (GPIOx == GPIOC){
		__HAL_RCC_GPIOC_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOD
	 if (GPIOx == GPIOD){
		__HAL_RCC_GPIOD_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOE
	 if (GPIOx == GPIOE){
		__HAL_RCC_GPIOE_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOF
	 if (GPIOx == GPIOF){
		__HAL_RCC_GPIOF_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOG
	 if (GPIOx == GPIOG){
		__HAL_RCC_GPIOG_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOH
	 if (GPIOx == GPIOH){
		__HAL_RCC_GPIOH_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOI
	 if (GPIOx == GPIOI){
		__HAL_RCC_GPIOI_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOJ
	 if (GPIOx == GPIOJ){
		__HAL_RCC_GPIOJ_CLK_DISABLE();
	}
	#endif
	#ifdef GPIOK
	 if (GPIOx == GPIOK){
		__HAL_RCC_GPIOK_CLK_DISABLE();
	}
	#endif
}	
