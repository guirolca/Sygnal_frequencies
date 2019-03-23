#include "ALE_CAN.h"
#include "ALE_USART.h"
extern ALE_USART_Handle alehusart;
//CAN_HandleTypeDef CAN_Handle;
/*
HAL_StatusTypeDef ALE_CAN_Config(CAN_HandleTypeDef *CAN_Handle){
	static CanTxMsgTypeDef CAN_TxMsg;
	static CanRxMsgTypeDef CAN_RxMsg;
	HAL_StatusTypeDef temp;
	GPIO_InitTypeDef GPIO_InitStruct;
	CAN_FilterConfTypeDef  sFilterConfig;
	__CAN_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(CAN_RX0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
	
	CAN_Handle->Instance = CAN;
	CAN_Handle->Init.TTCM=DISABLE;
	CAN_Handle->Init.ABOM=DISABLE;
	CAN_Handle->Init.AWUM=DISABLE;
	CAN_Handle->Init.NART=DISABLE;
	CAN_Handle->Init.RFLM=DISABLE;
	CAN_Handle->Init.TXFP=DISABLE;
	CAN_Handle->Init.Mode=CAN_MODE_NORMAL;
	CAN_Handle->Init.SJW = CAN_SJW_1TQ;
	CAN_Handle->Init.BS1= CAN_BS1_8TQ;
	CAN_Handle->Init.BS2= CAN_BS2_7TQ;
	CAN_Handle->Init.Prescaler= HAL_RCC_GetPCLK1Freq() / (16*1000000);
	CAN_Handle->pRxMsg = &CAN_RxMsg;
	CAN_Handle->pTxMsg = &CAN_TxMsg;
	temp = HAL_CAN_Init(CAN_Handle);
	if (temp != HAL_OK){
		return temp;
	}
	
	sFilterConfig.FilterNumber = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.BankNumber = 14;
  temp = HAL_CAN_ConfigFilter(CAN_Handle, &sFilterConfig);
  if (temp != HAL_OK){
		return temp;
	}

	
	temp = HAL_CAN_Receive_IT(CAN_Handle,CAN_FIFO0);
	return temp;
	
}
*/
/*
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle){
	ALE_CAN_ReceiveHandler(CanHandle);

 __HAL_CAN_ENABLE_IT(CanHandle,CAN_IT_FMP0);
	
}
*/
/*
void CAN_RX0_IRQHandler(void)
{
	
	HAL_CAN_IRQHandler(&CAN_Handle);
}
*/

void ALE_CAN_Init(CAN_HandleTypeDef *hcan, ALE_CAN_Handle *alehcan){
	alehcan->hcan=hcan;
	alehcan->tx_bufferCount=0;
	alehcan->tx_nextByte=0;
	alehcan->tx_bufferSize=0;
	alehcan->rx_bufferCount=0;
	alehcan->rxend = 1;
}

uint32_t ALE_CAN_Send(ALE_CAN_Handle *alehcan, CanTxMsgTypeDef *pTxMsg){
	uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;
	
	__HAL_LOCK(alehcan->hcan);
	/* Select one empty transmit mailbox */
	if((alehcan->hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0){
		transmitmailbox = 0;
	}else if((alehcan->hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1){
		transmitmailbox = 1;
	}else if((alehcan->hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2){
		transmitmailbox = 2;
	}
	if (transmitmailbox != CAN_TXSTATUS_NOMAILBOX){
		/* Set up the Id */
		alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
		if(pTxMsg->IDE == CAN_ID_STD){
			assert_param(IS_CAN_STDID(pTxMsg->StdId));  
			alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((pTxMsg->StdId << 21) | \
																								pTxMsg->RTR);
		}else{
			assert_param(IS_CAN_EXTID(pTxMsg->ExtId));
			alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((pTxMsg->ExtId << 3) | \
																								pTxMsg->IDE | \
																								pTxMsg->RTR);
		}
	
		/* Set up the DLC */
		pTxMsg->DLC &= (uint8_t)0x0000000F;
		alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= (uint32_t)0xFFFFFFF0;
		alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= pTxMsg->DLC;

		/* Set up the data field */
		alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TDLR = (((uint32_t)pTxMsg->Data[3] << 24) | 
																					 ((uint32_t)pTxMsg->Data[2] << 16) |
																					 ((uint32_t)pTxMsg->Data[1] << 8) | 
																					 ((uint32_t)pTxMsg->Data[0]));
		alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TDHR = (((uint32_t)pTxMsg->Data[7] << 24) | 
																					 ((uint32_t)pTxMsg->Data[6] << 16) |
																					 ((uint32_t)pTxMsg->Data[5] << 8) |
																					 ((uint32_t)pTxMsg->Data[4]));
	
		if(alehcan->hcan->State == HAL_CAN_STATE_BUSY_RX){
			/* Change CAN state */
			alehcan->hcan->State = HAL_CAN_STATE_BUSY_TX_RX;
		}else{
			/* Change CAN state */
			alehcan->hcan->State = HAL_CAN_STATE_BUSY_TX;
		}
		
		/* Set CAN error code to none */
		alehcan->hcan->ErrorCode = HAL_CAN_ERROR_NONE;
		
		/* Process Unlocked */
		__HAL_UNLOCK(alehcan->hcan);
		
		/* Enable Error warning Interrupt */
		__HAL_CAN_ENABLE_IT(alehcan->hcan, CAN_IT_EWG);
		
		/* Enable Error passive Interrupt */
		__HAL_CAN_ENABLE_IT(alehcan->hcan, CAN_IT_EPV);
		
		/* Enable Bus-off Interrupt */
		__HAL_CAN_ENABLE_IT(alehcan->hcan, CAN_IT_BOF);
		
		/* Enable Last error code Interrupt */
		__HAL_CAN_ENABLE_IT(alehcan->hcan, CAN_IT_LEC);
		
		/* Enable Error Interrupt */
		__HAL_CAN_ENABLE_IT(alehcan->hcan, CAN_IT_ERR);
		
		/* Enable Transmit mailbox empty Interrupt */
		__HAL_CAN_ENABLE_IT(alehcan->hcan, CAN_IT_TME);
		
		/* Request transmission */
		alehcan->hcan->Instance->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;
	}
	return transmitmailbox;
}




HAL_StatusTypeDef ALE_CAN_Transmit(ALE_CAN_Handle *alehcan, CanTxMsgTypeDef *pTxMsg){
	char str[100];
		//sprintf(str,"Entré>");
		//ALE_USART_Puts(&alehusart,str);
	
		sprintf(str,"MailBox = %d\nBusState = %d\n\n",ALE_CAN_GetMailbox(alehcan->hcan),(alehcan->hcan->State));
		ALE_USART_Puts(&alehusart,str);
	
	CanTxMsgTypeDef txMsgTemp = *pTxMsg;
	if (ALE_CAN_GetMailbox(alehcan->hcan) != CAN_TXSTATUS_NOMAILBOX && (alehcan->hcan->State == HAL_CAN_STATE_READY || alehcan->hcan->State == HAL_CAN_STATE_BUSY_RX)){
		sprintf(str,"\tEnvio\n--------\n");
		ALE_USART_Puts(&alehusart,str);	
		
		alehcan->hcan->pTxMsg = pTxMsg;
		HAL_CAN_Transmit_IT(alehcan->hcan);

	}else{
		sprintf(str,"Buffer\n--------\n");
		ALE_USART_Puts(&alehusart,str);	
		alehcan->tx_buffer[alehcan->tx_bufferCount++]=txMsgTemp;
		if (alehcan->tx_bufferCount == ALE_CAN_TX_MAX_BUFFER){
				alehcan->tx_bufferCount=0;
		}
		alehcan->tx_bufferSize++;
	}
}
/*	
	if((alehcan->hcan->State == HAL_CAN_STATE_READY) || (alehcan->hcan->State == HAL_CAN_STATE_BUSY_RX)){
		// Select one empty transmit mailbox //
    if (ALE_CAN_Send(alehcan,pTxMsg)==CAN_TXSTATUS_NOMAILBOX){
			alehcan->tx_buffer[alehcan->tx_bufferCount++]=*pTxMsg;
			if (alehcan->tx_bufferCount == ALE_CAN_TX_MAX_BUFFER){
				alehcan->tx_bufferCount=0;
			}
			alehcan->tx_bufferSize++;
		}
		return HAL_OK;
	}else{
		return HAL_BUSY;
	}
}
*/

void ALE_CAN_TX_IRQHandler(ALE_CAN_Handle *alehcan){
		char str[100];
		sprintf(str,"ALE_CAN_TX_IRQHandler");
		ALE_USART_Puts(&alehusart,str);
	CanTxMsgTypeDef pTxMsg;
	pTxMsg = alehcan->tx_buffer[alehcan->tx_nextByte];
	if (ALE_CAN_GetMailbox(alehcan->hcan)!= CAN_TXSTATUS_NOMAILBOX){
		alehcan->hcan->pTxMsg = &pTxMsg;
		HAL_CAN_Transmit_IT(alehcan->hcan);
		alehcan->tx_nextByte++;
		if (alehcan->tx_nextByte == ALE_CAN_TX_MAX_BUFFER){
			alehcan->tx_nextByte=0;
		}
		alehcan->tx_bufferSize--;
		if(alehcan->tx_bufferSize==0){
			__HAL_CAN_DISABLE_IT(alehcan->hcan,CAN_IT_TME);
		}
	}
}

void addCan32Bit(uint32_t value, CanTxMsgTypeDef *pTx, uint8_t offset){
	uint8_t i;
	for (i=0;i<4;i++){
		pTx->Data[i+offset] = (uint8_t)((value>>(32-(8*i+1))) & 0xFF);
	}
}

void addCan16Bit(uint16_t value, CanTxMsgTypeDef *pTx, uint8_t offset){
	uint8_t i;
	for (i=0;i<2;i++){
		pTx->Data[i+offset] = (uint8_t)((value>>(16-(8*i+1))) & 0xFF);
	}
}

void addCan8Bit(uint8_t value, CanTxMsgTypeDef *pTx, uint8_t offset){
		pTx->Data[offset] = (value);
}

uint32_t ALE_CAN_GetMailbox(CAN_HandleTypeDef* hcan){
	uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;
	if((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0){
		transmitmailbox = 0;
	}else if((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1){
		transmitmailbox = 1;
	}else if((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2){
		transmitmailbox = 2;
	}
	return transmitmailbox;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
