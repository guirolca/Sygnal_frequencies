#include "stm32f3xx.h"
#define ALE_CAN_TX_MAX_BUFFER 50
#define ALE_CAN_RX_MAX_BUFFER 20
typedef struct {
	CAN_HandleTypeDef *hcan;
	CanTxMsgTypeDef tx_buffer[ALE_CAN_TX_MAX_BUFFER];
	uint32_t tx_bufferCount;
	uint32_t tx_bufferSize;
	uint32_t tx_nextByte;
	CanRxMsgTypeDef rx_buffer[ALE_CAN_RX_MAX_BUFFER];
	CanRxMsgTypeDef  rx_buffer_user[ALE_CAN_RX_MAX_BUFFER];
	uint32_t rx_buffer_user_size;
	uint32_t rx_bufferCount;
	uint8_t rxend;
}ALE_CAN_Handle;


HAL_StatusTypeDef ALE_CAN_Config(CAN_HandleTypeDef *CAN_Handle);
void ALE_CAN_ReceiveHandler(CAN_HandleTypeDef *CAN_Handle);
void ALE_CAN_TX_IRQHandler(ALE_CAN_Handle *alehcan);
HAL_StatusTypeDef ALE_CAN_Transmit(ALE_CAN_Handle *alehcan, CanTxMsgTypeDef *pTxMsg);
uint32_t ALE_CAN_Send(ALE_CAN_Handle *alehcan, CanTxMsgTypeDef *pTxMsg);
void ALE_CAN_Init(CAN_HandleTypeDef *hcan, ALE_CAN_Handle *alehcan);
void addCan32Bit(uint32_t value, CanTxMsgTypeDef *pTx, uint8_t offset);
void addCan16Bit(uint16_t value, CanTxMsgTypeDef *pTx, uint8_t offset);
void addCan8Bit(uint8_t value, CanTxMsgTypeDef *pTx, uint8_t offset);
uint32_t ALE_CAN_GetMailbox(CAN_HandleTypeDef* hcan);
