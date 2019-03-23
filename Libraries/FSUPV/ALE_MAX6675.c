#include "ALE_MAX6675.h"
#include "tm_stm32_lcd.h"
#include "tm_stm32_delay.h"

void delayspi(uint32_t time){
	while (time--){};
}


void max6675_init(MAX6675_t *max6675, GPIO_TypeDef *GPIO_CS, uint16_t GPIO_CS_Pin, \
									GPIO_TypeDef *GPIO_SCK, uint16_t GPIO_SCK_Pin, \
									GPIO_TypeDef *GPIO_SO, uint16_t GPIO_SO_Pin) {
	max6675->GPIO_cs = GPIO_CS;
	max6675->GPIO_CS_pin = GPIO_CS_Pin;
	max6675->GPIO_sck = GPIO_SCK;
	max6675->GPIO_SCK_pin = GPIO_SCK_Pin;
	max6675->GPIO_so = GPIO_SO;
	max6675->GPIO_SO_pin = GPIO_SO_Pin;
										
	TM_GPIO_Init(max6675->GPIO_cs, max6675->GPIO_CS_pin, TM_GPIO_Mode_OUT, 
								TM_GPIO_OType_PP,TM_GPIO_PuPd_UP,TM_GPIO_Speed_High);
	TM_GPIO_Init(max6675->GPIO_sck, max6675->GPIO_SCK_pin, TM_GPIO_Mode_OUT, 
								TM_GPIO_OType_PP,TM_GPIO_PuPd_UP,TM_GPIO_Speed_High);
	TM_GPIO_Init(max6675->GPIO_so, max6675->GPIO_SO_pin, TM_GPIO_Mode_IN, 
								TM_GPIO_OType_PP,TM_GPIO_PuPd_UP,TM_GPIO_Speed_High);
	
	CS_H(max6675);
	SCK_L(max6675);
	
}

void max6675_init2(MAX6675_t *max6675) {
	
	TM_GPIO_Init(max6675->GPIO_cs, max6675->GPIO_CS_pin, TM_GPIO_Mode_OUT, 
								TM_GPIO_OType_PP,TM_GPIO_PuPd_UP,TM_GPIO_Speed_High);
	TM_GPIO_Init(max6675->GPIO_sck, max6675->GPIO_SCK_pin, TM_GPIO_Mode_OUT, 
								TM_GPIO_OType_PP,TM_GPIO_PuPd_UP,TM_GPIO_Speed_High);
	TM_GPIO_Init(max6675->GPIO_so, max6675->GPIO_SO_pin, TM_GPIO_Mode_IN, 
								TM_GPIO_OType_OD,TM_GPIO_PuPd_NOPULL,TM_GPIO_Speed_High);
	CS_H(max6675);
	SCK_L(max6675);
	
}



float readTemp(MAX6675_t *max6675){
	int i=0;
	uint16_t value=0;
	CS_L(max6675);
	//Delay(200);
	delayspi(21600);
	for (i=0;i<16;i++){
		SCK_H(max6675);
		//Delay(200);
		delayspi(21600);
		SCK_L(max6675);
		value = value<<1;
		if (HAL_GPIO_ReadPin(max6675->GPIO_so,max6675->GPIO_SO_pin)){
			value |= 0x0001;
		}
		//Delay(200);
		delayspi(21600);
	}
	CS_H(max6675);
	if (value & 0x8000){
		return -1;
	}
	if (value & 0x0004){
		return -2;
	}
	return (float)((value>>3) & 0x0FFF)/4.0f;
}
	
	
							
