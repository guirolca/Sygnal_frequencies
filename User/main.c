/**
 * Keil project example for USB MSC on both USB ports at the same time
 *
 * @note      Check defines.h file for configuration settings!
 *
 * Before you start, select your target, on the right of the "Load" button
 *
 * @author    Tilen Majerle
 * @email     tilen@majerle.eu
 * @website   http://stm32f4-discovery.com
 * @ide       Keil uVision 5
 * @conf      PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 * @packs     STM32F4xx/STM32F7xx Keil packs are requred with HAL driver support
 * @stdperiph STM32F4xx/STM32F7xx HAL drivers required
 */
/* Include core modules */
#include "stm32fxxx_hal.h"
/* Include my libraries here */
#include "defines.h"
#include "main.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_usb_host.h"
#include "tm_stm32_usb_host_msc.h"
#include "tm_stm32_lcd.h"
#include "tm_stm32_fatfs.h"
#include "tm_stm32_rtc.h"
#include "tm_stm32_mpu6050.h"
#include "tm_stm32f4_bkpsram.h"
#include "Funciones.h"
#include "ALE_USART_F4.h"
#include "ALE_GPIO_F4.h"
#include "ALE_Buffer.h"
#include "USBfunc.h"
#include "Config.h"
#include "ALE_TIM.h"
#include "Control.h"
#include "packet.h"
#include "TC.h"

TIM_HandleTypeDef htim3;
CAN_HandleTypeDef hcan2;
CAN_FilterConfTypeDef hcan2Filter;
CanRxMsgTypeDef RxMessage;
CanTxMsgTypeDef TxMessage;
USART_HandleTypeDef huart3;
USART_HandleTypeDef huart2;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim5; //beicon timer
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
ALE_USART_Handle ahus;
ALE_USART_Handle ahus2;

	
/* FATFS related */
FATFS FATFS_USB_FS, FATFS_USB_HS;
FIL fil;
FRESULT fres;

volatile uint32_t tiempo=0;
volatile uint16_t TPS=0, ECT=0, RPM=0, Lambda=0, MAP=0, MGP=0, OILSTATE=0, OILTEMP=0, MAF=0;
volatile uint16_t IAT=0, BAT=0, VSS=0, ACCFUEL=0;
volatile uint8_t GEAR, beacon=0, GEAR_CUT=0;
volatile uint16_t SWSens=0, CurrentSens=0, DamperRR=0, DamperRL=0;
volatile uint32_t batteryCurrent=0;
volatile uint16_t BrakePressF=0, BrakePressR=0, DamperFR=0, DamperFL=0;
volatile uint16_t TempRRTireO=0, TempRRTireM=0, TempRRTireI=0, TempRRCal=0, TempRRDisc=0;
volatile uint16_t TempRLTireO=0, TempRLTireM=0, TempRLTireI=0, TempRLCal=0, TempRLDisc=0;
volatile uint16_t TempFRTireO=0, TempFRTireM=0, TempFRTireI=0, TempFRCal=0, TempFRDisc=0;
volatile uint16_t TempFLTireO=0, TempFLTireM=0, TempFLTireI=0, TempFLCal=0, TempFLDisc=0;
volatile uint16_t TempFLTireMO=0, TempFLTireMI=0, TempFRTireMO=0, TempFRTireMI=0; 
volatile uint16_t TempRLTireMO=0, TempRLTireMI=0, TempRRTireMO=0, TempRRTireMI=0; 
volatile uint16_t speedRR_p, speedRL_p;
volatile uint16_t speedFR_p, speedFL_p;
volatile uint32_t countFL=0, countFR=0, countRL=0, countRR=0;
volatile uint32_t Caudalim_w=0, Caudalim_p=0;
volatile uint16_t Termopar_A=0, Termopar_B=0, Termopar_C=0, Termopar_D=0, Termopar_E=0, Termopar_F=0, Termopar_G=0, Termopar_H=0, Termopar_I=0, Termopar_J=0, Termopar_K=0, Termopar_L=0;
volatile uint16_t cellVoltage1=0,cellVoltage2=0,cellVoltage3=0,cellVoltage4=0, InjEPW=0;

//volatile uint16_t numberLap=0;
volatile uint8_t overTime=0;
volatile uint16_t lapTimeMilis=0;

BUFFER BUFF;

int SCHEDULER;
volatile UbloxM8N GPS;

char MOUNT[]="USBFS:";
//char OPEN[]="USBFS:000000000000.txt";
char OPEN[]="USBFS:Data0.txt";

TM_RTC_t RTCD;
TM_MPU6050_t IMU;

uint8_t sec = 0;
volatile uint8_t ADQOK=0;
volatile uint8_t USBRead=0;
volatile uint8_t telemetry_flag=0;
volatile uint8_t telemetry=0;
volatile uint32_t time=0;
volatile Selector_TC SelectorTC=TC_OFF;
volatile Selector_DAid SelectorDAid=DAid_OFF;
volatile TC_Parameters TC;

volatile uint8_t LCSFlag=0;
float slip_R, slip_L;

//lcs
volatile uint16_t RPMTARGET=10500;
volatile float LCSMAXSPEED=3.0f; //3(m/s)*3.6=10.8 km/h
volatile float kp=0.03f,ki=0.0f,kd=0.0f;
volatile uint8_t LCS_ON_OFF=0;

//shift
volatile uint8_t shiftstate=0;
volatile uint16_t shift_cut=600, shift_up=3400, shift_down=3400, antibounce=400;  //60ms 340ms 340ms 40ms
volatile uint16_t GEAR_UP_TIME_1 = 800;
volatile uint16_t GEAR_UP_TIME_2 = 800;
volatile uint16_t SPARK_CUT_TIME_1 = 500;
volatile uint16_t SPARK_CUT_TIME_2 = 500;
volatile uint8_t FTS_ON_OFF=1;

//BB
volatile uint16_t BB_Position;
volatile uint16_t BB_Target;
volatile uint8_t BB_ON_OFF=1;


//autoshift
volatile uint16_t RPM12=11750, RPM23=11700, RPM34=11700;
volatile uint8_t AS_ON_OFF=0;

extern uint8_t TC_flag;

volatile USB_MODE USB_SelectedMode = USB_DEVICE_MSC;
volatile USB_MODE USB_ActiveMode = USB_OFF;

volatile uint8_t USB_FLAG = 0;

//TM_USBD_CDC_Settings_t USB_FS_Settings;

CONFIG_AUX_FRONT configFront;
CONFIG_AUX_REAR configRear;
CONFIG_ESW configESW;
CONFIG_ADQ configADQ;

uint8_t usbWriteFlags = 0;


int main(void) 
{
//	char telemetry_data[30];
	uint16_t cnt=0;
	uint16_t cnt2=0;

	BUFF.countR=0;
	BUFF.countW=0;
	GPS.dataavailable=0;
  
	SCHEDULER=USBWait;
	
	//SystemClock_Config();
	TM_RCC_InitSystem();
	HAL_Init();
	MX_GPIO_Init();

//	RTC_Config();
	
	MX_CAN2_Init();
	hcan2.pRxMsg = &RxMessage;
	hcan2.pTxMsg = &TxMessage;
	HAL_CAN_Receive_IT(&hcan2,CAN_FIFO0);
	
	TIM3_Init();
	HAL_TIM_Base_Start_IT(&htim3); 
	
	TIM5_Init();
//	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
	
	UARTTelemetry_Config();
	packet_init(sendFunction,processPacket,0);
	
//	char ch = 'A';
//	char received[10];
//	uint8_t r_index=0;
	
//	uint8_t usb_state = 0;
		
	USB_Config();
//	TM_BKPSRAM_Init();
	
//	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	
//	readBackupSRAM();
	
	/* Check if values haven't been initializated */
//	if(!configADQ.SD) configADQ.SD = 20;
//	if(!configADQ.TELEMETRY) configADQ.TELEMETRY = 20;

	configADQ.SD = 20;
	configADQ.TELEMETRY = 20;
	
	while (1) 
	{	
		#ifdef ADQ
		  TM_USBH_Process(TM_USB_FS);
		  ALE_USART_Process_Byte(&ahus);
		#endif
		switch(SCHEDULER)
		{
			case USBWait:
					#ifdef ADQ
					  if(USBRead & 0x01) // ---> Cuando USBRead == 0x01   bien por USBRead = 0x01 & 0xFD o bien por USBRead = 0x02
					  {
						  	USBReadBlock();
								Variables_to_0();
							  cnt+=DATASIZE1;
							  if(cnt>BLOCKSIZE)
							  {
							  		cnt-=BLOCKSIZE;
							 	  	SCHEDULER=USBWrite;
							  }
								TC_flag=1;
								
							  USBRead &= 0xFE; // Reset flag
					  }
						
						
						if(USBRead & 0x02)
					  {
						  	USBReadBlock2();
							  cnt+=DATASIZE2;
							  if(cnt>BLOCKSIZE)
							  {
							  		cnt-=BLOCKSIZE;
							 	  	SCHEDULER=USBWrite;
							  }
							  USBRead &= 0xFD; // Reset flag
					  }

						
					  if(telemetry_flag==1)
					  {
						  sendTelemetry();
							telemetry_flag=0;
					  }
					#endif
			break;
						
			case USBWrite:
				_DISABLE_INTERRUPTS;
				USBWrite1024Block(&BUFF);  ////CRITAL AREA////
				_ENABLE_INTERRUPTS;
				SCHEDULER=USBWait;
			break;
		}	
	}
}


