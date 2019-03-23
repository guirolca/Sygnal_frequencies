#include "tm_stm32_gpio.h"
//#include "tm_stm32_delay.h"
#define CS_H(max) HAL_GPIO_WritePin(max->GPIO_cs,max->GPIO_CS_pin,GPIO_PIN_SET)
#define CS_L(max) HAL_GPIO_WritePin(max->GPIO_cs,max->GPIO_CS_pin,GPIO_PIN_RESET)
#define SCK_L(max) HAL_GPIO_WritePin(max->GPIO_sck,max->GPIO_SCK_pin,GPIO_PIN_SET)
#define SCK_H(max) HAL_GPIO_WritePin(max->GPIO_sck,max->GPIO_SCK_pin,GPIO_PIN_RESET)
#define SO_Read(max) HAL_GPIO_ReadPin(max->GPIO_so,max->GPIO_SO_pin)

typedef struct {
	GPIO_TypeDef *GPIO_cs;
	uint16_t GPIO_CS_pin;
	GPIO_TypeDef *GPIO_sck;
	uint16_t GPIO_SCK_pin;
	GPIO_TypeDef *GPIO_so;
	uint16_t GPIO_SO_pin;
}MAX6675_t;

void max6675_init(MAX6675_t *max6675, GPIO_TypeDef *GPIO_CS, uint16_t GPIO_CS_Pin, \
									GPIO_TypeDef *GPIO_SCK, uint16_t GPIO_SCK_Pin, \
									GPIO_TypeDef *GPIO_SO, uint16_t GPIO_SO_Pin);
void max6675_init2(MAX6675_t *max6675);

float readTemp(MAX6675_t *max6675);
