#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "stm32f4xx.h"
#include "main.h"

typedef enum
{
	USBWrite,
	USBDataCollect,
	USBWait,
	USBWaitingW
} Scheduler;

typedef enum {
	GPS_WAITING=0,
	GPS_IDENT,
	GPS_LAT,
	GPS_NS,
	GPS_LON,
	GPS_EW,
	GPS_UTC,
	GPS_STATUS,
	GPS_MODE,
	GPS_CHECKSUM,
}GPS_t;

typedef enum{
	LCS_CONFIG=0,
	FTS_CONFIG=1,
	AS_CONFIG=2,
	TELEMETRY_CMD=3,
	READ_LCS=4,
	READ_FTS=5,
	READ_AS=6,
	TC_CONFIG=7,
	READ_TC=8,
	FLAGS_CONFIG=9,
	PARAMETERS_CONFIG=10,
	LAPTIME=11,
	BeaconDetected=12,
	LaserTimingConfirmRecieved=13,
	ShowLaserTimeESW=14,
	LLANTA=15,
	BB_Config=16,
	READ_BB=17,
	DOWNSHIFT_CONF=18,
	DOWNSHIFT_EDIT=19
}COMMANDS;

typedef enum {
	USB_DEVICE_MSC,
	USB_DEVICE_CDC,
	USB_OFF
} USB_MODE;
typedef enum
{
	DAid_OFF=0,
	FTS,
	LC,
	FTS_AS,
	FTS_LC,
	FTS_LC_AS,
}Selector_DAid;
typedef enum
{
	TC_OFF=0,
	MODE1,
	MODE2,
	MODE3,
	MODE4,
	MODE5
}Selector_TC;
typedef struct {
	uint16_t MLX;
	uint16_t CAN;
	uint16_t DAMPER;
	uint16_t GPS;
} CONFIG_AUX_FRONT;

typedef struct {
	uint16_t MLX;
	uint16_t CAN;
	uint16_t DAMPER;
} CONFIG_AUX_REAR;

typedef struct {
	uint16_t DISPLAY;
	uint16_t TACHOMETER;
	uint16_t ADQ_TIMEOUT;
} CONFIG_ESW;

typedef struct {
	uint16_t SD;
	uint16_t TELEMETRY;
} CONFIG_ADQ;

typedef enum{ //Flags
  GreenFlag=0,
  RedFlag=1,
  YellowFlag=2,
  BlueFlag=3,
  WhiteFlag=4,
  BlackFlag=5,
  PermanentREDOP=6,
  PermanentREDECT=7,
}FLAGSENUM;

/*
enum DATA_BLOCK1 {
	TPS = 0,
	ECT,
	RPM,
	LAMBDA,
	IAT,
	BAT,
	VSS,
	ACCFUEL,
	MAP,
	MGP,
	MAF,
	OILSTATE,
	OILTEMP,

	SPEED_FR,
	SPEED_FL,
	SPEED_RR,
	SPEED_RL,

	BRAKEPRES_F,
	BRAKEPRES_R,

	TEMP_TIRE_FR_OUT,
	TEMP_TIRE_FR_MID,
	TEMP_TIRE_FR_IN,
	TEMP_TIRE_FL_OUT,
	TEMP_TIRE_FL_MID,
	TEMP_TIRE_FL_IN,
	TEMP_TIRE_RR_OUT,
	TEMP_TIRE_RR_MID,
	TEMP_TIRE_RR_IN,
	TEMP_TIRE_RL_OUT,
	TEMP_TIRE_RL_MID,
	TEMP_TIRE_RL_IN,

	TEMP_DISC_FR,
	TEMP_DISC_FL,
	TEMP_DISC_RR,
	TEMP_DISC_RL,

	IMU_ACC_X,
	IMU_ACC_Y,
	IMU_ACC_Z,
	IMU_GYRO_X,
	IMU_GYRO_Y,
	IMU_GYRO_Z,

	TEMP_TERMO1,
	TEMP_TERMO2,
	TEMP_TERMO3,
	TEMP_TERMO4,

	BEACON,

	GPS_LAT,
	GPS_LON,

	LAP_NUMBER,
	LAP_TIME,

	DATA_SIZE1
};

enum DATA_BLOCK2 {
	DAMPER_FR = 0,
	DAMPER_FL,
	DAMPER_RR,
	DAMPER_RL,
	DATA_SIZE2
};

typedef struct {
	uint8_t Sizes[DATA_SIZE2];
	uint8_t Data[
}

typedef struct {
	uint8_t Size;
	uint32_t Data;
} DATA_FIELD;
*/

void USBWrite1024Block(BUFFER* buffer);
void USBReadBlock(void);
void USBReadBlock2 (void);
void SendfromADQ(unsigned int id, unsigned char byte, unsigned char bit, unsigned char value);
void DisableAllInterrupts(void);
void EnableAllInterrupts(void);
void GPS_Config(void);
void processByte(unsigned char c);
unsigned long ReadCanData(CanRxMsgTypeDef* Msg, unsigned char lenght, unsigned char offset);
void ReadCANData(CanRxMsgTypeDef* Msg);
short ReadIMUCanData(CanRxMsgTypeDef* Msg, unsigned char offset, unsigned char len);
uint64_t Read64CanData(CanRxMsgTypeDef* Msg, unsigned char offset, unsigned char len);
void Variables_to_0(void);
int fputc(int ch, FILE* fil);
void LedForAero(uint16_t speedFL);
void GEAR_calc(void);

void processTelemetryByte(unsigned char c);
void sendFunction(unsigned char *data, unsigned int len);
void processPacket(unsigned char *data, unsigned int len);

void sendASData(void);
void sendFTSData(void);
void sendLCSData(void);
void sendTelemetry(void);
void sendTCData(void);
void sendBeaconDetected(uint32_t LapTime);
void sendFlagsData(int FLAGSENUM);
void sendBBData(void);
void sendDNData(void);

void USB_ChangeMode(USB_MODE mode);

void USB_ReadData(char* data);
void CAN_SendConfig(uint16_t ID, uint8_t val1, uint8_t val2);
void addCan16Bit(uint16_t value, CanTxMsgTypeDef *pTx, uint8_t offset);
void SendCanData(unsigned char DLC, unsigned short id, CanTxMsgTypeDef *pTx, CAN_HandleTypeDef* can);
void addCan32Bit(uint32_t value, CanTxMsgTypeDef *pTx, uint8_t offset);
void readBackupSRAM(void);
void configDataBlocks(void);
void SaveTimes(uint16_t TimeMilis, uint8_t NumOverLaps);


#endif
