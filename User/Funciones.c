#include "stm32f4xx_hal.h"
#include "tm_stm32_usb_host.h"
#include "tm_stm32_usb_host_msc.h"
#include "tm_stm32_fatfs.h"
#include "tm_stm32_rtc.h"
#include "tm_stm32_mpu6050.h"
#include "tm_stm32f4_bkpsram.h"
#include "mxconstants.h"
#include "main.h"
#include "defines.h"
#include "ALE_USART_F4.h"
#include "ALE_GPIO_F4.h"
#include "ALE_Buffer.h"
#include "ALE_GPS.h"
#include "Funciones.h"
#include "packet.h"
#include "TC.h"
#include "USBfunc.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern CAN_HandleTypeDef hcan2;
extern CAN_FilterConfTypeDef hcan2Filter;
extern CanRxMsgTypeDef RxMessage;
extern CanTxMsgTypeDef TxMessage;
extern USART_HandleTypeDef huart3;
extern USART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim7;
extern ALE_USART_Handle ahus;
extern ALE_USART_Handle ahus2;
extern Selector_TC SelectorTC;
extern Selector_DAid SelectorDAid;

extern FATFS FATFS_USB_FS, FATFS_USB_HS;
extern FIL fil;
extern FRESULT fres;
extern char MOUNT[];
extern char OPEN[];

extern uint16_t TPS, ECT, RPM, Lambda, MAP, MGP, OILSTATE, OILTEMP, MAF;
extern uint16_t IAT, BAT, VSS, ACCFUEL;
extern uint32_t SWSens, CurrentSens, DamperRR, DamperRL,batteryCurrent;
extern uint16_t BrakePressF, BrakePressR, DamperFR, DamperFL;
extern uint32_t tiempo,time;
extern uint8_t GEAR,beacon,GEAR_CUT;

extern BUFFER BUFF;
extern BUFFER BUFF2;
extern UbloxM8N GPS;
extern TM_RTC_t RTCD;
extern TM_MPU6050_t IMU;

extern uint16_t speedRR_p, speedRL_p;
extern uint16_t speedFR_p, speedFL_p;
extern uint16_t TempRRTireO, TempRRTireM, TempRRTireI, TempRRCal, TempRRDisc;
extern uint16_t TempRLTireO, TempRLTireM, TempRLTireI, TempRLCal, TempRLDisc;
extern uint16_t TempFRTireO, TempFRTireM, TempFRTireI, TempFRCal, TempFRDisc;
extern uint16_t TempFLTireO, TempFLTireM, TempFLTireI, TempFLCal, TempFLDisc;
extern uint16_t TempFLTireMO, TempFLTireMI, TempFRTireMO, TempFRTireMI; 
extern uint16_t TempRLTireMO, TempRLTireMI, TempRRTireMO, TempRRTireMI; 
extern uint32_t countFL, countFR, countRL, countRR;
extern uint32_t Caudalim_w, Caudalim_p;
extern uint16_t Termopar_A, Termopar_B, Termopar_C, Termopar_D, Termopar_E, Termopar_F, Termopar_G, Termopar_H, Termopar_I, Termopar_J, Termopar_K, Termopar_L;
extern uint16_t cellVoltage1,cellVoltage2,cellVoltage3,cellVoltage4, InjEPW;

extern uint8_t ADQOK;

//lcs
extern uint16_t RPMTARGET;
extern uint16_t LCSMAXSPEED;
extern float kp,ki,kd;
extern uint8_t LCS_ON_OFF;

//shift
extern uint8_t shiftstate;
//extern uint16_t shift_cut, shift_up, shift_down, antibounce;
extern uint16_t GEAR_UP_TIME_2;
extern uint16_t GEAR_UP_TIME_1;
extern uint16_t SPARK_CUT_TIME_1;
extern uint16_t SPARK_CUT_TIME_2;
extern uint8_t FTS_ON_OFF;

//BB
//extern uint8_t BB_Position;uint8_t BB_Target;uint8_t BB_ON_OFF=1;
extern volatile uint16_t BB_Position;
extern volatile uint16_t BB_Target;
extern volatile uint8_t BB_ON_OFF;

//autoshift 
extern uint16_t RPM12, RPM23, RPM34;
extern uint8_t AS_ON_OFF;

extern uint8_t shift_up_req, shift_dn_req, TC_req;
extern uint32_t TC_cut_time_req;
extern uint16_t SR_req, SR_Raw;

uint64_t GPS_RawData=0;
uint8_t i=0;
uint16_t TC_Combustions=0; 
float SR_target=0;
extern uint8_t TC_ON_OFF;

extern USB_MODE USB_SelectedMode;
extern USB_MODE USB_ActiveMode;
extern uint8_t USB_FLAG;

extern CONFIG_AUX_FRONT configFront;
extern CONFIG_AUX_REAR configRear;
extern CONFIG_ESW configESW;
extern CONFIG_ADQ configADQ;

//Laser Timing
extern uint8_t overTime;
extern uint16_t lapTimeMilis;
int8_t LaserOnESW=0;

uint16_t Time=0;
uint8_t Over=0;

//Confirm Beacons
uint8_t BeaconConfirmed=0;
extern uint32_t LapTime;

uint16_t TimeToStartDN=1000;
uint16_t TimeToHoldDN=800;

uint16_t CounterStrike=0;

void USBWrite1024Block(BUFFER* buffer)
{
	static uint8_t FS_Mounted=0,datax=0;
	uint16_t i;
	uint16_t ret=0;
	if (!FS_Mounted) 
	{
			//TM_RTC_GetDateTime(&RTCD, TM_RTC_Format_BIN);
			if (
			TM_USBH_IsConnected(TM_USB_FS) == TM_USBH_Result_Ok &&     /*!< Check if any device connected to USB FS port */
			TM_USBH_MSC_IsConnected(TM_USB_FS) == TM_USBH_Result_Ok && /*!< Device connected to USB port is MSC type */
			TM_USBH_MSC_IsReady(TM_USB_FS) == TM_USBH_Result_Ok        /*!< Device is ready */
			) {
					if ((fres = f_mount(&FATFS_USB_FS, MOUNT, 1)) == FR_OK) 
					{
							do {
//								sprintf(OPEN,"USBFS:%.2u%.2u%.2u%.2u%.2u%.2u.txt",RTCD.Year,RTCD.Month,RTCD.Day,RTCD.Hours,RTCD.Minutes,RTCD.Seconds);
//								if((fres = f_open(&fil, OPEN, FA_READ)) != FR_OK) {break;}
								sprintf(OPEN,"USBFS:Data%.2u.txt",datax);
								if((fres = f_open(&fil, OPEN, FA_READ)) != FR_OK) {break;}
								f_close(&fil);
								datax++;
							} while(1);	
							f_close(&fil);
							if ((fres = f_open(&fil, OPEN, FA_OPEN_ALWAYS | FA_WRITE | FA_READ)) == FR_OK)
							{		
//								f_expand(&fil, FILESIZE, 1);
								f_expand(&fil, FILESIZE, 0);
									FS_Mounted=1;
									ret=0;
									for(i=0; i<BLOCKSIZE;i++)
									{					
												ret+=f_putc(buffer->data[buffer->countW],&fil);
												buffer->countW++;
												if(buffer->countW==BUFFERSIZE) {buffer->countW=0;}
									}
									if(ret==BLOCKSIZE)
									{
											ToggleLedGreen;
											ADQOK=1;
											SendfromADQ(469,0,7,1);
									}
									else
									{
										  ADQOK=0;
											SendfromADQ(469,0,7,0);				
									}
							    f_sync(&fil);
							 } 
						}
					}
	}
	else if(TM_USBH_IsConnected(TM_USB_FS) == TM_USBH_Result_Ok)
	{
			ret=0;
			for(i=0; i<BLOCKSIZE;i++)
			{					
						ret+=f_putc(buffer->data[buffer->countW],&fil);
						buffer->countW++;
						if(buffer->countW==BUFFERSIZE) {buffer->countW=0;}
			}
			if(ret==BLOCKSIZE)
			{
					ToggleLedGreen;
					ADQOK=1;
					SendfromADQ(469,0,7,1);
			}
			else
			{
					ADQOK=0;
					SendfromADQ(469,0,7,0);				
			}
			f_sync(&fil); 
	} else {
		FS_Mounted = 0;
	}
}


void USBReadBlock(void)
{	
	addStarttobuffer(&BUFF, '1');     //5
	
	add32tobuffer(time,&BUFF);        //9
	
	add16tobuffer(TPS,&BUFF);        //11
	//add16tobuffer(ECT,&BUFF);        //13
	//add16tobuffer(RPM,&BUFF);				 //15
	//add16tobuffer(Lambda,&BUFF);     //17
	//add16tobuffer(IAT,&BUFF);				 //19
	//add16tobuffer(BAT,&BUFF);				 //21
	//add16tobuffer(VSS,&BUFF);				 //23
	//add16tobuffer(ACCFUEL,&BUFF);    //25
	//add16tobuffer(MAP,&BUFF);        //27
	//add16tobuffer(MGP,&BUFF); 			 //29
	//add16tobuffer(MAF,&BUFF); 			 //31
	//add16tobuffer(OILSTATE,&BUFF);	 //33
	//add16tobuffer(OILTEMP,&BUFF); 	 //35
	//add8tobuffer(GEAR_CUT,&BUFF);		 //36

	//add16tobuffer(speedFR_p,&BUFF);  //38
	//add16tobuffer(speedFL_p,&BUFF);  //40
	//add16tobuffer(speedRL_p,&BUFF);  //42
	//add16tobuffer(speedRR_p,&BUFF);  //44
	
	//add16tobuffer(DamperFR,&BUFF);	 //46
	//add16tobuffer(DamperFL,&BUFF);   //48
	//dd16tobuffer(DamperRL,&BUFF);   //50
	//add16tobuffer(DamperRR,&BUFF);   //52

	//add16tobuffer(BrakePressF,&BUFF);//54
	//add16tobuffer(BrakePressR,&BUFF);//56
	
	//add16tobuffer(TempFRTireO,&BUFF);//58
	//TempFRTireM=(TempFRTireMI+TempFRTireMO)/2;
	//add16tobuffer(TempFRTireM,&BUFF);//60
	//add16tobuffer(TempFRTireI,&BUFF);//62
	
	//add16tobuffer(TempFLTireO,&BUFF);//64
	//TempFLTireM=(TempFLTireMI+TempFLTireMO)/2;
	//add16tobuffer(TempFLTireM,&BUFF);//66
	//add16tobuffer(TempFLTireI,&BUFF);//68
	
	//add16tobuffer(TempRRTireO,&BUFF);//70
	//TempRRTireM=(TempRRTireMI+TempRRTireMO)/2;
	//add16tobuffer(TempRRTireM,&BUFF);//72
	//add16tobuffer(TempRRTireI,&BUFF);//74
	
	//add16tobuffer(TempRLTireO,&BUFF);//76
	//TempRLTireM=(TempRLTireMI+TempRLTireMO)/2;
	//add16tobuffer(TempRLTireM,&BUFF);//78
	//add16tobuffer(TempRLTireI,&BUFF);//80
	
	//add16tobuffer(TempFRDisc,&BUFF); //82
	//add16tobuffer(TempFLDisc,&BUFF); //84
	//add16tobuffer(TempRRDisc,&BUFF); //86
	//add16tobuffer(TempRLDisc,&BUFF); //88
	
	//add16tobuffer(SWSens,&BUFF);		 //90
	//add16tobuffer(CurrentSens,&BUFF);//92

	//add16tobuffer(IMU.Accelerometer_X,&BUFF); //94
	//add16tobuffer(IMU.Accelerometer_Y,&BUFF); //96
	//add16tobuffer(IMU.Accelerometer_Z,&BUFF); //98
	//add16tobuffer(IMU.Gyroscope_X,&BUFF);     //100
	//add16tobuffer(IMU.Gyroscope_Y,&BUFF); 		//102
	//add16tobuffer(IMU.Gyroscope_Z,&BUFF); 		//104
	
	//add16tobuffer(Termopar_A,&BUFF);		 			//106
	//add16tobuffer(Termopar_B,&BUFF);          //108
	//add16tobuffer(Termopar_C,&BUFF);   				//110
	//add16tobuffer(Termopar_D,&BUFF);   				//112
	
	//add16tobuffer(Termopar_E,&BUFF); 					//114
	//add16tobuffer(Termopar_F,&BUFF); 					//116
	//add16tobuffer(Termopar_G,&BUFF); 					//118
	//add16tobuffer(Termopar_H,&BUFF); 					//120	
	
	//add16tobuffer(Termopar_I,&BUFF); 					//122
	//add16tobuffer(Termopar_J,&BUFF); 					//124
	//add16tobuffer(Termopar_K,&BUFF); 					//126
	//add16tobuffer(Termopar_L,&BUFF); 					//128
	
//	add16tobuffer(cellVoltage1,&BUFF);				//109
//	add16tobuffer(cellVoltage2,&BUFF);				//111
//	add16tobuffer(cellVoltage3,&BUFF);				//113
//	add16tobuffer(cellVoltage4,&BUFF);				//115
//	
//	add32tobuffer(batteryCurrent,&BUFF);			//119
		
	//add8tobuffer(beacon,&BUFF);							//129
	
	//add8tobuffer(shift_up_req,&BUFF);         //130
	//add8tobuffer(shift_dn_req,&BUFF);         //131
	//add16tobuffer(InjEPW,&BUFF);							//133
  //add8tobuffer(SelectorDAid,&BUFF);
//	add8tobuffer(GEAR,&BUFF);                 //180
//	add32tobuffer(TC_cut_time_req,&BUFF);     //184
//	add8tobuffer(SR_target,&BUFF);						//185
//	add8tobuffer(TC_Combustions,&BUFF);				//186
//	add8tobuffer(TC_req,&BUFF);	             	//187
//	add16tobuffer(SR_req,&BUFF);              //189
//	add16tobuffer(SR_Raw,&BUFF); 							//191

		//add16tobuffer(lapTimeMilis,&BUFF);				//135
    //add8tobuffer(overTime,&BUFF);							//136
		
		//add8tobuffer(GPS.GNGLL[0],&BUFF);					//137
		//add8tobuffer(GPS.GNGLL[1],&BUFF);					//138
		//add8tobuffer(GPS.GNGLL[2],&BUFF);					//139
		//add8tobuffer(GPS.GNGLL[3],&BUFF);					//140
		//add8tobuffer(GPS.GNGLL[4],&BUFF);					//141
		//add8tobuffer(GPS.GNGLL[5],&BUFF);					//142
		//add8tobuffer(GPS.GNGLL[6],&BUFF);					//143
		//add8tobuffer(GPS.GNGLL[7],&BUFF); 			  //144
	
	
//	addStoptobuffer(&BUFF);          				


	
	//add32tobuffer(time,&BUFF);     //9	
	
	//add16tobuffer(speedFR_p,&BUFF);  //11
	//add16tobuffer(speedFL_p,&BUFF);  //13
	//add16tobuffer(speedRL_p,&BUFF);  //15
	//add16tobuffer(speedRR_p,&BUFF);  //17
	
	//add16tobuffer(DamperFR,&BUFF);	 //19
	//add16tobuffer(DamperFL,&BUFF);   //21
	//add16tobuffer(DamperRL,&BUFF);   //23
	//add16tobuffer(DamperRR,&BUFF);   //25
	
//	addStoptobuffer(&BUFF);          //21
}


void ReadCANData(CanRxMsgTypeDef* Msg) //Read CAN Messages
{
	switch (Msg->StdId) //Read the StdId to know the content of the message 
	{	
			  ///////////// ECU DATA //////////////
				case 100:
							TPS=ReadCanData(Msg,0,2);
							RPM=ReadCanData(Msg,2,2);
							Lambda=ReadCanData(Msg,4,2);
							ECT=ReadCanData(Msg,6,2);
				break;
				case 110:
							BAT=ReadCanData(Msg,0,2);
							IAT=ReadCanData(Msg,2,2);
							ACCFUEL=ReadCanData(Msg,4,2);
							VSS=ReadCanData(Msg,6,2);
							GEAR_calc();
				break;
				case 120:
							MAP=ReadCanData(Msg,0,2);
							MGP=ReadCanData(Msg,2,2);
							OILSTATE=ReadCanData(Msg,4,2);
							GEAR_CUT=ReadCanData(Msg,6,1);
				break;
				case 130:
							OILTEMP=ReadCanData(Msg,0,2);
							MAF=ReadCanData(Msg,2,2);
							InjEPW=ReadCanData(Msg,4,2);
				break;
				///////// FRONT MODULE DATA /////////
				case 220:
							DamperFR=ReadCanData(Msg,0,2);
							BrakePressF=ReadCanData(Msg,2,2);
							DamperFL=ReadCanData(Msg,4,2);
							SWSens=ReadCanData(Msg,6,2);
				break;
				case 230:
							TempFRCal=ReadCanData(Msg,0,2);
							TempFRDisc=ReadCanData(Msg,2,2);
							TempFLCal=ReadCanData(Msg,4,2);
							TempFLDisc=ReadCanData(Msg,6,2);
				break;
				case 270:
							GPS_RawData=Read64CanData(Msg,0,8);
							for(i=0;i<8;i++)
							{
								GPS.GNGLL[i]=(GPS_RawData>>(56-8*i))&0xFF;
							}		
				break;
				///////// REAR MODULE DATA /////////
				case 320:
							DamperRR=ReadCanData(Msg,0,2);
							DamperRL=ReadCanData(Msg,2,2);
							BrakePressR=ReadCanData(Msg,4,2);	
				break;
				case 330:
  						TempRRCal=ReadCanData(Msg,0,2);
							TempRRDisc=ReadCanData(Msg,2,2);
							TempRLCal=ReadCanData(Msg,4,2);
							TempRLDisc=ReadCanData(Msg,6,2);
				break;
//				case 360:
//							//Caudalim_w=ReadCanData(Msg,0,2);
//							//beacon=ReadCanData(Msg,2,1);
//				break;
				///////// EXTRA MODULE DATA /////////
				case 400:
							Termopar_A=ReadCanData(Msg,0,2);
							Termopar_B=ReadCanData(Msg,2,2);
							Termopar_C=ReadCanData(Msg,4,2);
							Termopar_D=ReadCanData(Msg,6,2);
				break;
				case 410:
							CurrentSens=ReadCanData(Msg,0,2);
				break;
				case 411:
							Termopar_E=ReadCanData(Msg,0,2);
							Termopar_F=ReadCanData(Msg,2,2);
							Termopar_G=ReadCanData(Msg,4,2);
							Termopar_H=ReadCanData(Msg,6,2);
				
							Termopar_I=ReadCanData(Msg,8,2);
							Termopar_J=ReadCanData(Msg,10,2);
							Termopar_K=ReadCanData(Msg,12,2);
							Termopar_L=ReadCanData(Msg,14,2);
				break;
				case 420:
							batteryCurrent=ReadCanData(Msg,0,4);
							cellVoltage1=ReadCanData(Msg,4,2);
							cellVoltage2=ReadCanData(Msg,6,2);
							cellVoltage3=ReadCanData(Msg,8,2);
							cellVoltage4=ReadCanData(Msg,10,2);				
				break;
//				case 430:
//							tempBoardBattery=ReadCanData(Msg,0,2);
//							tempExt1=ReadCanData(Msg,2,2);
//							tempExt2=ReadCanData(Msg,4,2);
//				break;
				/////////DRIVER AID MODULE///////////
				case 500:
							SelectorTC=ReadCanData(Msg,0,1);
							SelectorDAid=ReadCanData(Msg,1,1);
							
				break;
				case 610:
							speedFR_p=ReadCanData(Msg,0,2);
							speedFL_p=ReadCanData(Msg,2,2);
							speedRR_p=ReadCanData(Msg,4,2);
							speedRL_p=ReadCanData(Msg,6,2);
							CounterStrike++;
				break;
				case 620:
							IMU.Accelerometer_X=ReadIMUCanData(Msg,0,2);
							IMU.Accelerometer_Y=ReadIMUCanData(Msg,2,2);
							IMU.Accelerometer_Z=ReadIMUCanData(Msg,4,2);
							IMU.Gyroscope_X=ReadIMUCanData(Msg,6,2);
				break;
				case 630:
							IMU.Gyroscope_Y=ReadIMUCanData(Msg,0,2);
							IMU.Gyroscope_Z=ReadIMUCanData(Msg,2,2);
							TC_cut_time_req=ReadCanData(Msg,4,2);
							if(!shift_up_req) shift_up_req=ReadCanData(Msg,6,1);
							if(!shift_dn_req) shift_dn_req=ReadCanData(Msg,7,1);
				break;
				case 640:
							SR_req=ReadCanData(Msg,0,2);
							TC_Combustions=ReadCanData(Msg,2,2);
							SR_target=ReadCanData(Msg,4,2);
							SR_Raw=ReadCanData(Msg,6,2);
				break;
				////////////////////////////////////
				case 700:
						TempFRTireO=ReadCanData(Msg,0,2);
						TempFRTireMO=ReadCanData(Msg,2,2);
						TempFRTireMI=ReadCanData(Msg,4,2);
						TempFRTireI=ReadCanData(Msg,6,2);
				break;
				case 704:
					  TempFLTireO=ReadCanData(Msg,0,2);
						TempFLTireMO=ReadCanData(Msg,2,2);
						TempFLTireMI=ReadCanData(Msg,4,2);
						TempFLTireI=ReadCanData(Msg,6,2);		
				break;
				case 708:
					  TempRRTireO=ReadCanData(Msg,0,2);
						TempRRTireMO=ReadCanData(Msg,2,2);
						TempRRTireMI=ReadCanData(Msg,4,2);
						TempRRTireI=ReadCanData(Msg,6,2);		
				break;
				case 712:
					  TempRLTireO=ReadCanData(Msg,0,2);
						TempRLTireMO=ReadCanData(Msg,2,2);
						TempRLTireMI=ReadCanData(Msg,4,2);
						TempRLTireI=ReadCanData(Msg,6,2);	
				break;
	} 
}


void GPS_Config(void)
{
	ahus2.baudrate=9600;
	ahus2.GPIO_Rx=GPIOA;
	ahus2.GPIO_Tx=GPIOA;
	ahus2.GPIO_Rx_pin=GPIO_Pin_3;
	ahus2.GPIO_Tx_pin=GPIO_Pin_2;
	ahus2.USARTx = USART2;
	ahus2.process_byte_func = processByte;
	ALE_USART_Init(&ahus2,&huart2);
}


/*
//void gps_procces_byte(unsigned char c){
//	static unsigned char ident[10], lat[15], lon[15];
//	static unsigned char ident_cnt=0, temp_cnt=0;
//	static float latf, lonf;
//	switch (GPS.state){
//		case GPS_WAITING:
//			if (c=='$'){
//				GPS.state=GPS_IDENT;
//				ident_cnt=0;
//				memset(ident,0,10);
//			}
//			break;
//		case GPS_IDENT:
//			if (c != ','){
//				ident[ident_cnt++]=c;
//			}else{
//				if (ident[0]=='G' && ident[2]=='G' && ident[3]=='L' && ident[4]=='L'){
//					GPS.state=GPS_LAT;
//					memset(lat,0,15);
//					temp_cnt=0;
//				}else{
//					GPS.state=GPS_WAITING;
//				}
//			}
//			break;
//		case GPS_LAT:
//			if (c != ','){
//					lat[temp_cnt++]=c;
//			}else{
//				latf=str2float(lat);
//				GPS.state=GPS_NS;
//				
//			}
//			break;
//		case GPS_NS:
//			if (c==','){
//				GPS.state=GPS_LON;
//				memset(lon,0,15);
//				temp_cnt=0;
//			}else if (c=='N' || c=='S'){
//				if (c=='S'){
//					latf *=-1.0f;
//				}
//			}else{
//				GPS.state=GPS_WAITING;
//			}
//			break;
//		case GPS_LON:
//			if (c != ','){
//					lon[temp_cnt++]=c;
//			}else{
//				lonf=str2float(lon);
//				GPS.state=GPS_EW;
//				
//			}
//			break;
//		case GPS_EW:
//			if (c==','){
//				GPS.state=GPS_UTC;
//				temp_cnt=0;
//			}else if (c=='E' || c=='W'){
//				if (c=='W'){
//					lonf *=-1.0f;
//				}
//			}else{
//				GPS.state=GPS_WAITING;
//			}
//			break;
//		case GPS_UTC:
//			if (c==','){
//				GPS.state=GPS_STATUS;
//			}
//			break;
//		case GPS_STATUS:
//			if (c==','){
//				GPS.state=GPS_MODE;
//			}else if (c=='A' || c=='V'){
//				GPS.datavalid=c;
//			}else{
//				GPS.datavalid=0;
//				GPS.state=GPS_WAITING;
//			}
//			break;
//		case GPS_MODE:
//			if (c==','){
//				GPS.state=GPS_WAITING;
//			}else if (c=='A' || c=='D' || c=='E' || c=='M' || c=='S' || c=='N'){
//				GPS.datamode=c;
//			}else{
//				GPS.datamode=0;
//				GPS.state=GPS_WAITING;
//			}
//			break;
//	}
//}*/
			

void processByte(unsigned char c)
{
	static unsigned char ident[6];
	static uint8_t ident_cnt=0,comacount=0;
	static uint16_t GNGLL_cnt=0;
	
	switch(GPS.state)
	{
			case 0:
				if(c=='$')
				{
					GPS.state++;
					ident_cnt=0;
				}
			break;
			case 1:
			  ident[ident_cnt++]=c;
				if(ident_cnt==6)
				{
					if(ident[0]=='G' && ident[2]=='G' && ident[3]=='L' && ident[4]=='L' && ident[5]==',')
					{
					   GPS.state++;
						 memset(GPS.GNGLL,0,24);					
						 comacount=0;
					}
					else
					{
						GPS.state=0;
					}
				}
			break;
			case 2:
					switch(c)
					{
						case ',':
							comacount++;
							if(comacount==4)
							{
								GPS.dataavailable=1;
								GPS.state=0;
								GNGLL_cnt=0;
							}
						break;
						case '$':
							GPS.state=1;
						  GNGLL_cnt=0;
					    ident_cnt=0;
						break;
						default:
							GPS.GNGLL[GNGLL_cnt++]=c;
						 break;
					}
			break;
	}
}


void SendfromADQ(unsigned int id, unsigned char databyte, unsigned char bit, unsigned char value)
{
		TxMessage.StdId=id;
		TxMessage.DLC=1;
		TxMessage.Data[databyte]=(value<<(bit)); 
		HAL_CAN_Transmit_IT(&hcan2);
}

void DisableAllInterrupts(void)
{
	//HAL_NVIC_DisableIRQ(TIM3_IRQn);
	HAL_NVIC_DisableIRQ(TIM7_IRQn);
	HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
	HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
	HAL_NVIC_DisableIRQ(USART2_IRQn);
	HAL_NVIC_DisableIRQ(USART3_IRQn);
	HAL_NVIC_DisableIRQ(TIM5_IRQn);
}

void EnableAllInterrupts(void)
{
	//HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}


unsigned long ReadCanData(CanRxMsgTypeDef* Msg, unsigned char offset, unsigned char len)
{
		uint32_t value=0;
		uint8_t i;
		for(i=0;i<len;i++)
		{
				value += (((uint32_t)(Msg->Data[offset+i])&0xFF)<<(8*(len-1-i)));
		}
		return value;
}

uint64_t Read64CanData(CanRxMsgTypeDef* Msg, unsigned char offset, unsigned char len)
{
		uint64_t value=0;
		uint8_t i;
		for(i=0;i<len;i++)
		{
				value += (((uint64_t)(Msg->Data[offset+i])&0xFF)<<(8*(len-1-i)));
		}
		return value;
}

short ReadIMUCanData(CanRxMsgTypeDef* Msg, unsigned char offset, unsigned char len)
{
		short value=0;
		uint8_t i;
		for(i=0;i<len;i++)
		{
				value += (((Msg->Data[offset+i])&0xFF)<<(8*(len-1-i)));
		}
		return value;
}

int fputc(int ch, FILE* fil)  //printf handler, DON'T DELETE THIS FUNCTION!
{  
	#if defined(STM32F429_DISCOVERY) || defined(STM32F7_DISCOVERY) || defined(STM32F439_EVAL)
		if (TM_LCD_Putc(ch) != TM_LCD_Result_Ok) 
		{
			TM_LCD_Fill(LCD_COLOR_WHITE);
			TM_LCD_SetXY(5, 5);
			TM_LCD_Putc(ch);
		}
	#endif
	return ch;
}

void Variables_to_0(void)
{
	beacon=0;
	shift_up_req=0;
	shift_dn_req=0;
	TC_cut_time_req=0;
	TC_req=0;
	SR_req=0;
	overTime=0;
	lapTimeMilis=0;
}

void processTelemetryByte(unsigned char c)
{
	packet_process_byte(c,0);
}

void sendFunction(unsigned char *data, unsigned int len)
{
	int i=0;
	for (i=0;i<len;i++){
		ALE_USART_Putc(&ahus,data[i]);
	}
}

void processPacket(unsigned char *data, unsigned int len)
{
	uint8_t packet_id;
	uint32_t ind=0;
	char configData[20];
	
	packet_id = data[0];
	data++;
	switch (packet_id){
		case LCS_CONFIG:
			ind=0;
			RPMTARGET = buffer2int16(data,&ind);
			LCSMAXSPEED = buffer2float16(data,&ind,1000.0f);
			kp = buffer2float16(data,&ind,1000.0f);
			ki = buffer2float16(data,&ind,1000.0f);
			kd = buffer2float16(data,&ind,1000.0f);
			LCS_ON_OFF = data[ind++];
			sendLCSData();
		
		break;
		case FTS_CONFIG:
			ind=0;
			SPARK_CUT_TIME_1 = buffer2int16(data,&ind);
			SPARK_CUT_TIME_2 = buffer2int16(data,&ind);
			GEAR_UP_TIME_1 = buffer2int16(data,&ind);
			GEAR_UP_TIME_2 = buffer2int16(data,&ind);
			FTS_ON_OFF = data[ind++];
			sendFTSData();
		break;
		case AS_CONFIG:
			ind=0;
			RPM12 = buffer2int16(data,&ind);
			RPM23 = buffer2int16(data,&ind);
			RPM34 = buffer2int16(data,&ind);
			AS_ON_OFF = data[ind++];
			sendASData();
			break;
		case READ_LCS:
			sendLCSData();
			break;
		case READ_FTS:
			sendFTSData();
			break;
		case READ_AS:
			sendASData();
			break;
		case TELEMETRY_CMD:
			sendTelemetry();
			break;
		case TC_CONFIG:
			ind=0;
			TC_Combustions = buffer2int16(data,&ind);
			SR_target = buffer2float16(data,&ind,100.0f);
			TC_ON_OFF = data[ind++];
			sendTCData();
			break;
		case READ_TC:
			sendTCData();
			break;
		case FLAGS_CONFIG:
			sendFlagsData(buffer2int16(data,&ind));
			break;
		case PARAMETERS_CONFIG:
			configData[0] = 0x60;
			configData[1] = 0x30;
			configData[2] = data[ind++];
			configData[3] = data[ind++];
			configData[4] = data[ind++];
			configData[5] = data[ind++];
		
			USB_ReadData(configData);
			break;
		case LAPTIME: //For laser times
			lapTimeMilis = buffer2int16(data,&ind);
			overTime = data[ind++];
			SaveTimes(lapTimeMilis, overTime);		
		  break;
		case ShowLaserTimeESW: //Causes showing or not the time of the laser on ESW (Steering Wheel) 
			LaserOnESW=buffer2int16(data,&ind);
			//LaserOnESW is used to tell the system if it have to send or not the beacon time
		break;
		case BeaconDetected:
			BeaconConfirmed=1;
		break;
		case LLANTA:
				ind=0;
				addCan16Bit(data[ind++], hcan2.pTxMsg, 0);
				SendCanData(2, 540, &TxMessage, &hcan2);
		break;
	 case BB_Config:
			ind=0;
			BB_Position = buffer2int16(data,&ind);
			BB_Target = buffer2int16(data,&ind);
			BB_ON_OFF = data[ind++];
			sendBBData();
		break;
		case READ_BB:
			sendBBData();
			break;
		case DOWNSHIFT_CONF:
			ind=0;
			TimeToStartDN = buffer2int16(data,&ind);
			TimeToHoldDN = buffer2int16(data,&ind);
		
			addCan16Bit(TimeToStartDN, hcan2.pTxMsg, 0); 
			addCan16Bit(TimeToHoldDN, hcan2.pTxMsg, 2); 

			SendCanData(4, 515, &TxMessage, &hcan2);	
			break;
		case DOWNSHIFT_EDIT:
			sendDNData();
			break;
	}
}



void sendTelemetry(void)
{
	uint8_t txData[300];
	unsigned int ind=0;
	txData[ind++]=TELEMETRY_CMD;
	buffer_append_int32(txData,time,&ind);
	buffer_append_int16(txData,TPS,&ind);
	buffer_append_int16(txData,RPM,&ind);
	buffer_append_int16(txData,ECT,&ind);
	buffer_append_int16(txData,Lambda,&ind);
	buffer_append_int16(txData,BAT,&ind);
	buffer_append_int16(txData,MAP,&ind);
	buffer_append_int16(txData,IAT,&ind);
//	buffer_append_int16(txData,MAF,&ind);
//	buffer_append_int16(txData,OILTEMP,&ind);
	buffer_append_int16(txData,speedFL_p,&ind);
	buffer_append_int16(txData,speedFR_p,&ind);
	buffer_append_int16(txData,speedRL_p,&ind);
	buffer_append_int16(txData,speedRR_p,&ind);
//	buffer_append_int16(txData,CurrentSens,&ind);
	buffer_append_int16(txData,TempFLTireI,&ind);
	buffer_append_int16(txData,TempFLTireM,&ind);
	buffer_append_int16(txData,TempFLTireO,&ind);
	buffer_append_int16(txData,TempFLDisc,&ind);	
	
	buffer_append_int16(txData,TempFRTireI,&ind);
	buffer_append_int16(txData,TempFRTireM,&ind);
	buffer_append_int16(txData,TempFRTireO,&ind);
	buffer_append_int16(txData,TempFRDisc,&ind);
	
	buffer_append_int16(txData,TempRLTireI,&ind);
	buffer_append_int16(txData,TempRLTireM,&ind);
	buffer_append_int16(txData,TempRLTireO,&ind);
	buffer_append_int16(txData,TempRLDisc,&ind);
	
	buffer_append_int16(txData,TempRRTireI,&ind);
	buffer_append_int16(txData,TempRRTireM,&ind);
	buffer_append_int16(txData,TempRRTireO,&ind);
	buffer_append_int16(txData,TempRRDisc,&ind);
	
	buffer_append_int16(txData,DamperFL,&ind);
	buffer_append_int16(txData,DamperFR,&ind);
	buffer_append_int16(txData,DamperRL,&ind);
	buffer_append_int16(txData,DamperRR,&ind);
	
	buffer_append_int16(txData,OILSTATE,&ind);
	
	buffer_append_int16(txData,IMU.Accelerometer_X,&ind);
	buffer_append_int16(txData,IMU.Accelerometer_Y,&ind);
	buffer_append_int16(txData,IMU.Gyroscope_Z,&ind);
	
	buffer_append_int16(txData,VSS,&ind);
	
	buffer_append_int16(txData,BrakePressF,&ind);
	buffer_append_int16(txData,BrakePressR,&ind);
	
	txData[ind++]=ADQOK;
	packet_send_packet(txData,ind,0);
}



void sendLCSData(void)
{
	uint8_t txData[100];
	unsigned int ind=0;
	txData[ind++]=LCS_CONFIG;
	buffer_append_int16(txData,RPMTARGET,&ind);
	buffer_append_int16(txData,(uint16_t)(LCSMAXSPEED*1000.0f),&ind);
	buffer_append_int16(txData,(uint16_t)(kp*1000.0f),&ind);
	buffer_append_int16(txData,(uint16_t)(ki*1000.0f),&ind);
	buffer_append_int16(txData,(uint16_t)(kd*1000.0f),&ind);
	txData[ind++]=LCS_ON_OFF;
	packet_send_packet(txData,ind,0);
	
	addCan16Bit((uint16_t)(kp*1000.0f), hcan2.pTxMsg, 0); 
	addCan16Bit((uint16_t)(ki*1000.0f), hcan2.pTxMsg, 2);
	addCan16Bit((uint16_t)(LCSMAXSPEED*1000.0f), hcan2.pTxMsg, 4); 
	addCan16Bit(RPMTARGET, hcan2.pTxMsg, 6);
	
	SendCanData(8, 510, &TxMessage, &hcan2);
}

void sendFTSData(void)
{
	uint8_t txData[50];
	unsigned int ind=0;
	txData[ind++]=FTS_CONFIG;
	buffer_append_int16(txData,SPARK_CUT_TIME_1,&ind);
	buffer_append_int16(txData,SPARK_CUT_TIME_2,&ind);
	buffer_append_int16(txData,GEAR_UP_TIME_1,&ind);
	buffer_append_int16(txData,GEAR_UP_TIME_2,&ind);
	txData[ind++]=FTS_ON_OFF;
	packet_send_packet(txData,ind,0);
	
	addCan16Bit(SPARK_CUT_TIME_1, hcan2.pTxMsg, 0); 
	addCan16Bit(SPARK_CUT_TIME_2, hcan2.pTxMsg, 2); 
	addCan16Bit(GEAR_UP_TIME_1, hcan2.pTxMsg, 4);
	addCan16Bit(GEAR_UP_TIME_2, hcan2.pTxMsg, 6); 
	
	SendCanData(8, 530, &TxMessage, &hcan2);		
}

void sendBBData(void)
{
	uint8_t txData[50];
	unsigned int ind=0;
	txData[ind++]=BB_Config;
	buffer_append_int16(txData,BB_Position,&ind);
	buffer_append_int16(txData,BB_Target,&ind);
	txData[ind++]=BB_ON_OFF;
	packet_send_packet(txData,ind,0);
	
	
	addCan16Bit(BB_Target, hcan2.pTxMsg,0);
	SendCanData(2, 277, &TxMessage, &hcan2);
	
	
}

void sendASData(void)
{
	uint8_t txData[100];
	unsigned int ind=0;
	txData[ind++]=AS_CONFIG;
	buffer_append_int16(txData,RPM12,&ind);
	buffer_append_int16(txData,RPM23,&ind);
	buffer_append_int16(txData,RPM34,&ind);
	txData[ind++]=AS_ON_OFF;
	packet_send_packet(txData,ind,0);
}

void sendTCData(void)
{
	uint8_t txData[100];
	unsigned int ind=0;
	txData[ind++]=TC_CONFIG;
	buffer_append_int16(txData,TC_Combustions,&ind);
	buffer_append_int16(txData,SR_target*100,&ind);
	txData[ind++]=TC_ON_OFF;
	packet_send_packet(txData,ind,0);
	
	addCan16Bit(TC_Combustions, hcan2.pTxMsg, 0); 
	addCan16Bit(SR_target*100, hcan2.pTxMsg, 2);
	TxMessage.Data[4] = TC_ON_OFF;
	
	SendCanData(5, 520, &TxMessage, &hcan2);
}

void sendDNData(void)
{
	uint8_t txData[100];
	unsigned int ind=0;
	txData[ind++]=DOWNSHIFT_CONF;
	buffer_append_int16(txData,TimeToStartDN,&ind);
	buffer_append_int16(txData,TimeToHoldDN,&ind);
	packet_send_packet(txData,ind,0);
	
	addCan16Bit(TimeToStartDN, hcan2.pTxMsg, 0); 
	addCan16Bit(TimeToHoldDN, hcan2.pTxMsg, 2); 

	SendCanData(4, 515, &TxMessage, &hcan2);	
}

void sendBeaconDetected(uint32_t LapTime)
{
	uint8_t txData[20];
	unsigned int ind=0;
	txData[ind++]=BeaconDetected;
	buffer_append_int32(txData,LapTime,&ind);
	packet_send_packet(txData,ind,0);
}

void sendFlagsData(int FLAGSENUM)
{
	
	switch (FLAGSENUM){
		
		case GreenFlag:
		SendfromADQ(470,0,0,0); // id, databyte, bite, value
//		uint8_t txData[20];
//		unsigned int ind=0;
//		txData[ind++]=FlagRecieved;
////		buffer_append_int16(txData,GreenFlag,&ind); Just in case you want to send the recieved color
//		packet_send_packet(txData,ind,0);
		break;
		
		case RedFlag:
		SendfromADQ(471,0,0,0);
		break;
		
		case YellowFlag:
		SendfromADQ(472,0,0,0);
		break;
		
		case WhiteFlag:
		SendfromADQ(473,0,0,0);
		break;
		
		case BlackFlag:
		SendfromADQ(474,0,0,0);
		break;
		
		case BlueFlag:
		SendfromADQ(475,0,0,0);
		break;
	}
	
}

void LedForAero(uint16_t speedFL)
{
	uint16_t result=0;
	result=(speedFL/1000);
	if(result>=17 && result<=20)
	{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
	}
	else
	{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	}
}
	

void GEAR_calc(void)
{
	float gear=0.0f;
	if (VSS>0){
		gear = ((float)RPM*10.0f)/(float)(VSS);
	}else{
		gear=13.0f;
	}
	if (gear>=10.0f && gear<=12.2f){
		GEAR=1;
	}else if (gear>=8.0f && gear<10){
		GEAR=2;
	}else if (gear>=6.85f && gear<8.0f){
		GEAR=3;
	}else if (gear>=6.0f && gear <6.85f){
		GEAR=4;
	}else if (gear<6.0f){
		GEAR=5;
	}else{
		GEAR=0;
	}
}

/*
//void USB_ChangeMode(USB_MODE mode)
//{
//	USB_ActiveMode = mode;
//	USB_FLAG = 1;
//	
//	switch(mode)
//	{
//		case USB_DEVICE_MSC:
//			TM_USB_Init();	
//			TM_USBD_MSC_Init(TM_USB_FS);
//			TM_USBD_Start(TM_USB_FS);
//			f_mount(NULL, "SD:", 1);
//		break;
//		
//		case USB_DEVICE_CDC:
//			TM_USB_Init();
//			TM_USBD_CDC_Init(TM_USB_FS);
//			TM_USBD_Start(TM_USB_FS);
//			f_mount(NULL, "SD:", 1);
//		break;
//		
//		case USB_OFF:
//			TM_USBD_Stop(TM_USB_FS);
//		break;	
//	}
//}*/

void USB_ReadData(char* data)
{
	if(data[0] == 0x60)
	{
		if(data[1] == 0x30)
		{
			uint16_t value = 0;
			value |= ((uint16_t) data[4] << 8);
			value |= (data[5]);
			
			switch(data[2])
			{
				case 0x10: // AUX_FRONT
					switch(data[3])
					{
						case 0x01:
							configFront.MLX = value;
							TM_BKPSRAM_Write16(AF_MLX90614_FREQ_ADDRESS, value);
						break;
						case 0x02:
							configFront.CAN = value;
						TM_BKPSRAM_Write16(AF_CAN_TRANSMIT_FREQ_ADDRESS, value);
						break;
						case 0x03:
							configFront.DAMPER = value;
							TM_BKPSRAM_Write16(AF_CAN_DAMPER_FREQ_ADDRESS, value);
						break;
						case 0x04:
							configFront.GPS = value;
							TM_BKPSRAM_Write16(AF_GPS_READ_DATA_FREQ_ADDRESS, value);
						break;
						default:
						break;
					}
					addCan16Bit(configFront.MLX, &TxMessage, 0); 
					addCan16Bit(configFront.CAN, &TxMessage, 2);
					addCan16Bit(configFront.DAMPER, &TxMessage, 4);
					addCan16Bit(configFront.GPS, &TxMessage, 6);
					SendCanData(8,1000,&TxMessage,&hcan2);
				break;
					
				case 0x20: // AUX_REAR
					switch(data[3])
					{
						case 0x01:
							configRear.MLX = value;
							TM_BKPSRAM_Write16(AR_MLX90614_FREQ_ADDRESS, value);
						break;
						case 0x02:
							configRear.CAN = value;
							TM_BKPSRAM_Write16(AR_CAN_TRANSMIT_FREQ_ADDRESS, value);
						break;
						case 0x03:
							configRear.DAMPER = value;
							TM_BKPSRAM_Write16(AR_CAN_DAMPER_FREQ_ADDRESS, value);
						break;
						default:
						break;
					}
					addCan16Bit(configRear.MLX, &TxMessage, 0); 
					addCan16Bit(configRear.CAN, &TxMessage, 2);
					addCan16Bit(configRear.DAMPER, &TxMessage, 4);
					SendCanData(6,1010,&TxMessage,&hcan2);
				break;
					
				case 0x30: //ESW
					switch(data[3])
					{
						case 0x01:
							configESW.DISPLAY = value;
							TM_BKPSRAM_Write16(ESW_DISPLAY_UPDATE_FREQ_ADDRESS, value);
						break;
						case 0x02:
							configESW.TACHOMETER = value;
							TM_BKPSRAM_Write16(ESW_TACHOMETER_BLINK_FREQ_ADDRESS, value);
						break;
						case 0x03:
							configESW.ADQ_TIMEOUT = value;
							TM_BKPSRAM_Write16(ESW_ADQ_TIMEOUT_ADDRESS, value);
						break;
						default:
						break;
					}
					addCan16Bit(configESW.DISPLAY, &TxMessage, 0); 
					addCan16Bit(configESW.TACHOMETER, &TxMessage, 2);
					addCan16Bit(configESW.ADQ_TIMEOUT, &TxMessage, 4);
					SendCanData(6,1020,&TxMessage,&hcan2);
				break;
					
				case 0x40: // ADQQ
					switch(data[3])
					{
						case 0x01:
							configADQ.SD = value;
							TM_BKPSRAM_Write16(ADQ_SD_WRITE_FREQ_ADDRESS, value);
						break;
						case 0x02:
							configADQ.TELEMETRY = value;
							TM_BKPSRAM_Write16(ADQ_TELEMETRY_FREQ_ADDRESS, value);
						break;
						default:
						break;
					}
					addCan16Bit(configADQ.SD, &TxMessage, 0); 
					addCan16Bit(configADQ.TELEMETRY, &TxMessage, 2);
					SendCanData(4,1030,&TxMessage,&hcan2);
				break;
			}
		}
	}
}

void addCan16Bit(uint16_t value, CanTxMsgTypeDef *pTx, uint8_t offset)
{
	uint8_t i;
	for (i=0;i<2;i++)
	{
		pTx->Data[i+offset] = ((value>>(16-(8*(i+1)))) & 0xFF);
	}
}

void addCan32Bit(uint32_t value, CanTxMsgTypeDef *pTx, uint8_t offset)
{
	uint8_t i;
	for (i=0;i<4;i++)
	{
		pTx->Data[i+offset] = ((value>>(32-(8*(i+1)))) & 0xFF);
	}
}

void SendCanData(unsigned char DLC, unsigned short id, CanTxMsgTypeDef *pTx, CAN_HandleTypeDef* can)
{
	can->pTxMsg->DLC=DLC;
	can->pTxMsg->StdId=id;
	HAL_CAN_Transmit_IT(can);
}

void readBackupSRAM(void)
{
	/* AUX_FRONT */
	configFront.MLX = TM_BKPSRAM_Read16(AF_MLX90614_FREQ_ADDRESS);
	configFront.CAN = TM_BKPSRAM_Read16(AF_CAN_TRANSMIT_FREQ_ADDRESS);
	configFront.DAMPER = TM_BKPSRAM_Read16(AF_CAN_DAMPER_FREQ_ADDRESS);
	configFront.GPS = TM_BKPSRAM_Read16(AF_GPS_READ_DATA_FREQ_ADDRESS);
	
	/* AUX_REAR */
	configRear.MLX = TM_BKPSRAM_Read16(AR_MLX90614_FREQ_ADDRESS);
	configRear.CAN = TM_BKPSRAM_Read16(AR_CAN_TRANSMIT_FREQ_ADDRESS);
	configRear.DAMPER = TM_BKPSRAM_Read16(AR_CAN_DAMPER_FREQ_ADDRESS);
	
	/* ESW */
	configESW.DISPLAY = TM_BKPSRAM_Read16(ESW_DISPLAY_UPDATE_FREQ_ADDRESS);
	configESW.TACHOMETER = TM_BKPSRAM_Read16(ESW_TACHOMETER_BLINK_FREQ_ADDRESS);
	configESW.ADQ_TIMEOUT = TM_BKPSRAM_Read16(ESW_ADQ_TIMEOUT_ADDRESS);
	
	/* ADQ */
	configADQ.SD = TM_BKPSRAM_Read16(ADQ_SD_WRITE_FREQ_ADDRESS);
	configADQ.TELEMETRY = TM_BKPSRAM_Read16(ADQ_TELEMETRY_FREQ_ADDRESS);
}

void SaveTimes(uint16_t TimeMilis, uint8_t NumOverLaps)
{
	uint8_t txData[20];

	unsigned int ind=0;
	txData[ind++]=LaserTimingConfirmRecieved;
  txData[ind++]=overTime;
	buffer_append_int16(txData,TimeMilis,&ind);
	packet_send_packet(txData,ind,0);
			
//					if (LaserOnESW==1&&lapTimeMilis!=0) 
//			{
//					addCan16Bit(lapTimeMilis,hcan2.pTxMsg,0);
//					SendCanData(6,800,&TxMessage, &hcan2);
//			} 
}



/*
void configDataBlocks(void)
{
	BUFF.sizes[TPS] = 16;
	BUFF.sizes[ECT] = 16;
	BUFF.sizes[RPM] = 16;
	BUFF.sizes[LAMBDA] = 16;
	BUFF.sizes[IAT] = 16;
	BUFF.sizes[BAT] = 16;
	BUFF.sizes[VSS] = 16;
	BUFF.sizes[ACCFUEL] = 16;
	BUFF.sizes[MAP] = 16;
	BUFF.sizes[MGP] = 16;
	BUFF.sizes[MAF] = 16;
	BUFF.sizes[OILSTATE] = 16;
	BUFF.sizes[OILTEMP] = 16;
	BUFF.sizes[SPEED_FR] = 16;
	BUFF.sizes[SPEED_FL] = 16;
	BUFF.sizes[SPEED_RR] = 16;
	BUFF.sizes[SPEED_RL] = 16;
	BUFF.sizes[BRAKEPRES_F] = 16;
	BUFF.sizes[BRAKEPRES_R] = 16;
	BUFF.sizes[TEMP_TIRE_FR_OUT] = 16;
	BUFF.sizes[TEMP_TIRE_FR_MID] = 16;
	BUFF.sizes[TEMP_TIRE_FR_IN] = 16;
	BUFF.sizes[TEMP_TIRE_FL_OUT] = 16;
	BUFF.sizes[TEMP_TIRE_FL_MID] = 16;
	BUFF.sizes[TEMP_TIRE_FL_IN] = 16;
	BUFF.sizes[TEMP_TIRE_RR_OUT] = 16;
	BUFF.sizes[TEMP_TIRE_RR_MID] = 16;
	BUFF.sizes[TEMP_TIRE_RR_IN] = 16;
	BUFF.sizes[TEMP_TIRE_RL_OUT] = 16;
	BUFF.sizes[TEMP_TIRE_RL_MID] = 16;
	BUFF.sizes[TEMP_TIRE_RL_IN] = 16;
	BUFF.sizes[TEMP_DISC_FR] = 16;
	BUFF.sizes[TEMP_DISC_FL] = 16;
	BUFF.sizes[TEMP_DISC_RR] = 16;
	BUFF.sizes[TEMP_DISC_RL] = 16;
	BUFF.sizes[IMU_ACC_X] = 16;
	BUFF.sizes[IMU_ACC_Y] = 16;
	BUFF.sizes[IMU_ACC_Z] = 16;
	BUFF.sizes[IMU_GYRO_X] = 16;
	BUFF.sizes[IMU_GYRO_Y] = 16;
	BUFF.sizes[IMU_GYRO_Z] = 16;
	BUFF.sizes[TEMP_TERMO1] = 16;
	BUFF.sizes[TEMP_TERMO2] = 16;
	BUFF.sizes[TEMP_TERMO3] = 16;
	BUFF.sizes[TEMP_TERMO4] = 16;
	BUFF.sizes[BEACON] = 8;
	BUFF.sizes[GPS_LAT] = 32;
	BUFF.sizes[GPS_LON] = 32;
	BUFF.sizes[LAP_NUMBER] = 16;
	BUFF.sizes[LAP_TIME] = 32;

	BUFF2.sizes[DAMPER_FR] = 16;
	BUFF2.sizes[DAMPER_FL] = 16;
	BUFF2.sizes[DAMPER_RR] = 16;
	BUFF2.sizes[DAMPER_RL] = 16;
}
*/
