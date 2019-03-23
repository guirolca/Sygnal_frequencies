#ifndef ALE_USART_F4_H
#define ALE_USART_F4_H


#include "stm32fxxx_hal.h"

#ifndef MAXTXBUFFER
	#define MAXTXBUFFER 200
#endif
#ifndef MAXRXBUFFER
	#define MAXRXBUFFER 200
#endif

typedef struct{
	USART_HandleTypeDef *hus;
	uint32_t tx_next;
	uint32_t tx_count;
	uint32_t rx_next;
	uint32_t rx_count;
	uint32_t tx_ind;
	uint32_t rx_ind;
	uint8_t tx_buffer[MAXTXBUFFER];
	uint8_t rx_buffer[MAXRXBUFFER];
	uint8_t dataavailable;
	uint8_t txwaiting;
	GPIO_TypeDef *GPIO_Tx;
	GPIO_TypeDef *GPIO_Rx;
	uint16_t GPIO_Tx_pin;
	uint16_t GPIO_Rx_pin;
	USART_TypeDef *USARTx;
	uint32_t baudrate;
	void(*process_byte_func)(unsigned char data);
}ALE_USART_Handle;

void ALE_USART_Init(ALE_USART_Handle *ahus, USART_HandleTypeDef *hus);
void ALE_USART_Puts(ALE_USART_Handle *husart, char *str);
void ALE_USART_Putc(ALE_USART_Handle *ahus, uint8_t c);
void ALE_USART_TxHandler(ALE_USART_Handle *ahus);
void ALE_USART_RxHandler(ALE_USART_Handle *ahus);
void ALE_USART_IRQHandler(ALE_USART_Handle *ahus);
void ALE_USART_Process_Byte(ALE_USART_Handle *husart);

#endif
