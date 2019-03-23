#include "ALE_USART_F1.h"
#include "ALE_GPIO_F1.h"
//#include "tm_stm32_lcd.h"
//#include "packet.h"
extern uint32_t timeIntH, timeIntL;

void ALE_USART_Putc(ALE_USART_Handle *ahus, uint8_t c){
		ahus->tx_buffer[ahus->tx_ind++]=c;
		ahus->hus->Instance->CR1 |= USART_CR1_TCIE;
		ahus->hus->Instance->CR1 |= USART_CR1_TE;
	
		ahus->txwaiting=1;
		if (ahus->tx_ind == MAXTXBUFFER){
			ahus->tx_ind=0;
		}
}


	

void ALE_USART_IRQHandler(ALE_USART_Handle *ahus){
	uint32_t sr,cr1,dr,i;
	sr = ahus->USARTx->SR;
	cr1 = ahus->USARTx->CR1;
	dr = ahus->USARTx->DR;
	if ((sr & USART_SR_TC) == USART_SR_TC && (cr1 & USART_CR1_TCIE) == USART_CR1_TCIE){ 
		if (ahus->tx_ind != ahus->tx_next){
			
			ahus->USARTx->DR = (uint8_t)ahus->tx_buffer[ahus->tx_next++];
			if (ahus->tx_next == MAXTXBUFFER){
				ahus->tx_next=0;
			}
		}else{
			ahus->USARTx->CR1 &= ~(USART_CR1_TCIE);
			//ahus->USARTx->CR1 &= ~(USART_CR1_TE);
			//HAL_GPIO_WritePin(ahus->GPIO_Tx,ahus->GPIO_Tx_pin,GPIO_PIN_SET);
		}
		
	}
	if (((sr & USART_SR_RXNE) == USART_SR_RXNE)){
			ahus->rx_buffer[ahus->rx_next++] = dr;
			if (ahus->rx_next == MAXRXBUFFER){
				ahus->rx_next=0;
			}
			ahus->dataavailable=1;
	}
	
}



void ALE_USART_Puts(ALE_USART_Handle *husart, char *str){
	while (*str){
		ALE_USART_Putc(husart, (uint8_t)(*str++));
	}
}

void ALE_USART_Init(ALE_USART_Handle *ahus,UART_HandleTypeDef *hus,
USART_TypeDef *USARTx, uint32_t baudrate, GPIO_TypeDef *GPIO_Tx, GPIO_TypeDef *GPIO_Rx,
uint16_t GPIO_Tx_pin, uint16_t GPIO_Rx_pin, void(*process_byte_func)(unsigned char data)){
	GPIO_InitTypeDef	gpiost;
//	uint32_t alternate_AF;
	IRQn_Type irq;
		ahus->USARTx = USARTx;
	ahus->baudrate=baudrate;
	ahus->GPIO_Rx = GPIO_Rx;
	ahus->GPIO_Tx = GPIO_Tx;
	ahus->GPIO_Tx_pin = GPIO_Tx_pin;
	ahus->GPIO_Rx_pin = GPIO_Rx_pin;
	ahus->process_byte_func = process_byte_func;
	//	TM_LCD_Puts("A1\n");
	ALE_GPIO_Clk_Enable(ahus->GPIO_Rx);
	ALE_GPIO_Clk_Enable(ahus->GPIO_Tx);
	__HAL_RCC_AFIO_CLK_ENABLE();
	//TM_LCD_Puts("A2\n");
	#ifdef USART1
	if (ahus->USARTx == USART1){
		irq = USART1_IRQn;
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();
	}
	#endif
	#ifdef USART2
	if (ahus->USARTx == USART2) {
		irq = USART2_IRQn;
		
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_USART2_FORCE_RESET();
		__HAL_RCC_USART2_RELEASE_RESET();
		__HAL_AFIO_REMAP_USART2_DISABLE();
	}
	#endif
	#ifdef USART3
	if (ahus->USARTx == USART3) {
		irq = USART3_IRQn;
		__HAL_RCC_USART3_CLK_ENABLE();
		__HAL_RCC_USART3_FORCE_RESET();
		__HAL_RCC_USART3_RELEASE_RESET();
	}
	#endif
	#ifdef UART4
	if (ahus->USARTx == UART4) {
		irq = UART4_IRQn;
		__HAL_RCC_UART4_CLK_ENABLE();
		__HAL_RCC_UART4_FORCE_RESET();
		__HAL_RCC_UART4_RELEASE_RESET();
	}
	#endif
	#ifdef UART5 
	if (ahus->USARTx == UART5) {
		irq = UART5_IRQn;
		__HAL_RCC_UART5_CLK_ENABLE();
		__HAL_RCC_UART5_FORCE_RESET();
		__HAL_RCC_UART5_RELEASE_RESET();
	}
	#endif
	#ifdef USART6
	if (ahus->USARTx == USART6){
		alternate_AF = GPIO_AF8_USART6;
		irq = USART6_IRQn;
		__HAL_RCC_USART6_CLK_ENABLE();
		__HAL_RCC_USART6_FORCE_RESET();
		__HAL_RCC_USART6_RELEASE_RESET();
	}
	#endif
	#ifdef UART7 
	if (ahus->USARTx == UART7) {
		alternate_AF = GPIO_AF8_UART7;
		irq = UART7_IRQn;
		__HAL_RCC_UART7_CLK_ENABLE();
		__HAL_RCC_UART7_FORCE_RESET();
		__HAL_RCC_UART7_RELEASE_RESET();
	}
	#endif
	#ifdef UART8
	if (ahus->USARTx == UART8){
		alternate_AF = GPIO_AF8_UART8;
		irq = UART8_IRQn;
		__HAL_RCC_UART8_CLK_ENABLE();
		__HAL_RCC_UART8_FORCE_RESET();
		__HAL_RCC_UART8_RELEASE_RESET();
	}
	#endif
	

	//TM_LCD_Puts("A3\n");
	gpiost.Pin = ahus->GPIO_Tx_pin;
	
	//gpiost.Alternate = alternate_AF;
	gpiost.Mode = GPIO_MODE_AF_PP;
	gpiost.Pull = GPIO_PULLUP;
	gpiost.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ahus->GPIO_Tx,&gpiost);
	//TM_LCD_Puts("A4\n");
	gpiost.Pin = ahus->GPIO_Rx_pin;
	//gpiost.Alternate = alternate_AF;
	gpiost.Mode = GPIO_MODE_AF_PP;//GPIO_MODE_INPUT;
	gpiost.Pull = GPIO_NOPULL;
	//gpiost.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ahus->GPIO_Rx,&gpiost);
	
	  
	//TM_LCD_Puts("A5\n");
	HAL_NVIC_SetPriority(irq, 0, 0);
  HAL_NVIC_EnableIRQ(irq);
	//TM_LCD_Puts("A6\n");


	hus->Instance = ahus->USARTx;
	hus->Init.BaudRate = ahus->baudrate;
	hus->Init.Mode = UART_MODE_TX_RX;
	hus->Init.Parity = UART_PARITY_NONE;
	hus->Init.StopBits = UART_STOPBITS_1;
	hus->Init.WordLength = UART_WORDLENGTH_8B;
	hus->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hus->Init.OverSampling = UART_OVERSAMPLING_16;
	
		//TM_LCD_Puts("A7\n");

	if (HAL_UART_Init(hus)!=HAL_OK){
		return;
		//	TM_LCD_Puts("HAL_ERROR\n");
	}
	ahus->USARTx->CR1 &= ~USART_CR1_UE;
	ahus->USARTx->CR1 |= USART_CR1_RXNEIE;
	ahus->USARTx->CR2 &= (uint32_t)~(uint32_t)USART_CR2_CLKEN;
	ahus->USARTx->CR2 |= USART_CR2_LINEN;
	ahus->USARTx->CR2 |= USART_CR2_CPOL;
	
	ahus->USARTx->CR1 |= USART_CR1_UE;
	
	
	//TM_LCD_Puts("RXNEIE = 1");
	
	ahus->hus = hus;
	ahus->rx_ind=0;
	ahus->rx_count=0;
	ahus->rx_next=0;
	ahus->tx_ind=0;
	ahus->tx_count=0;
	ahus->tx_next=0;
	ahus->dataavailable=0;
}
/*
void ALE_USART_ChangeBaud(ALE_USART_Handle *ahus, uint32_t baud){
	ahus->baudrate=baud;
	HAL_UART_DeInit(ahus->hus);
	ahus->hus->Instance = ahus->USARTx;
	ahus->hus->Init.BaudRate = ahus->baudrate;
	ahus->hus->Init.Mode = UART_MODE_TX_RX;
	ahus->hus->Init.Parity = UART_PARITY_NONE;
	ahus->hus->Init.StopBits = UART_STOPBITS_1;
	ahus->hus->Init.WordLength = UART_WORDLENGTH_8B;
	ahus->hus->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	ahus->hus->Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(ahus->hus)!=HAL_OK){
		return;
		//	TM_LCD_Puts("HAL_ERROR\n");
	}
	ahus->USARTx->CR1 &= ~USART_CR1_UE;
	ahus->USARTx->CR1 |= USART_CR1_RXNEIE;
	//ahus->USARTx->CR2 &= (uint32_t)~(uint32_t)USART_CR2_CLKEN;
	//ahus->USARTx->CR2 |= USART_CR2_LINEN;
	ahus->USARTx->CR1 |= USART_CR1_UE;
	//TM_LCD_Puts("RXNEIE = 1");
	

	ahus->rx_ind=0;
	ahus->rx_count=0;
	ahus->rx_next=0;
	ahus->tx_ind=0;
	ahus->tx_count=0;
	ahus->tx_next=0;
	ahus->dataavailable=0;
}
	*/
void ALE_USART_Process_Byte(ALE_USART_Handle *husart){
	if (husart->dataavailable){
		while(husart->rx_ind != husart->rx_next){
			if (husart->process_byte_func){
				husart->process_byte_func(husart->rx_buffer[husart->rx_ind++]);
			}
			//packet_process_byte(husart->rx_buffer[husart->rx_ind++],0);
			if (husart->rx_ind == MAXRXBUFFER){
				husart->rx_ind =0;
			}
		}
		husart->dataavailable=0;
	}
}
