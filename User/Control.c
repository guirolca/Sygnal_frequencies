#include "stm32f4xx_hal.h"
#include "defines.h"
#include "mxconstants.h"
#include "ALE_USART_F4.h"
#include "ALE_GPIO_F4.h"
#include "ALE_TIM.h"
#include "Control.h"

extern uint8_t shiftstate;

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;

//lcs
extern uint16_t RPMTARGET;
extern uint16_t LCSMAXSPEED;
extern float kp,ki,kd;
extern uint8_t LCS_ON_OFF;

//shift
extern uint8_t shiftstate;
extern uint16_t shift_cut, shift_up, shift_down, antibounce;
extern uint8_t FTS_ON_OFF;
extern uint8_t shift_up_req, shift_down_req;

//autoshift 
extern uint16_t RPM12, RPM23, RPM34;
extern uint8_t AS_ON_OFF;


//void EXTI1_IRQHandler(void)
//{
//	  HAL_GPIO_EXTI_IRQHandler(SHIN1_Pin);
//		shift_up_fcn();
//		shift_up_req=1;
//}

//void EXTI2_IRQHandler(void)
//{
//	HAL_GPIO_EXTI_IRQHandler(SHIN2_Pin);
//	shift_dn_fcn();
//	shift_down_req=1;
//}

void shift_up_fcn(){
	if(shiftstate == 0)
	{
		if (FTS_ON_OFF){
			ALE_TIM_SetPeriod(TIM6,shift_cut-1); //70 ms cut time
			CUT_ON;
		}
		ALE_TIM_SetPeriod(TIM4,shift_up-1); //340ms shift up time
		SHIFT_UP_ON;
		shiftstate=1;
		ALE_TIM_Start_IT(TIM6);
		ALE_TIM_Start_IT(TIM4);		
	}
}		

void shift_dn_fcn(){
	if (shiftstate == 0)
		{
		ALE_TIM_SetPeriod(TIM4,shift_down-1); //340ms shift dn time
		SHIFT_DN_ON;
		shiftstate = 2;
		ALE_TIM_Start_IT(TIM4);
	}
}

//void TIM6_DAC_IRQHandler(void){
//	LedGreenOff;
//	CUT_OFF;
//	ALE_TIM_Stop_IT(TIM6);
//	HAL_TIM_IRQHandler(&htim6);
//}




void LCS(float speedF, float error, float time){
	static float lasterror=0, lasttime=0, integral=0;
	float deriv;
	float dt;
	float output;
	uint32_t time2cut;
	
	dt = time-lasttime;
	lasttime=time;
	deriv = (error-lasterror)/dt;
	lasterror=error;
	if(speedF<=LCSMAXSPEED)
	{
	  integral +=error*dt;
	  integral = integral > MAXINTEGRAL ? MAXINTEGRAL : integral;
	  integral = integral < MININTEGRAL ? MININTEGRAL : integral;
	  output=(kp*error)+(ki*integral)+(kd*deriv);
	  output = output > 99.0f ? 99.0f : output;
	  output = output < 0.0f ? 0.0f : output;
	  time2cut = (uint32_t)(100.0f*output*dt); 
	  if (time2cut>0 && shiftstate==0)
		{
			 LedGreenOn;
		   ALE_TIM_SetPeriod(TIM6,time2cut);
		   CUT_ON;
		   ALE_TIM_Start_IT(TIM6);
	  }
	}
}

/*
1-2 61km/h 16.94m/s  ->  11750
2-3 84km/h 23.33m/s  ->  11750
3-4 100km/h 27.77m/s ->  11750
*/
#define speedMIN12 15.0f
#define speedMAX12 18.0f
#define RPMSHIFT12 11750

#define speedMIN23 21.0f
#define speedMAX23 25.0f
#define RPMSHIFT23 11750

#define speedMIN34 26.0f
#define speedMAX34 29.0f
#define RPMSHIFT34 11750

void autoShift (uint16_t RPM, float frontSpeed){
	
	if (frontSpeed>speedMIN12 /*&& frontSpeed<speedMAX12*/ && RPM>RPM12){
		shift_up_fcn();
	}
	if (frontSpeed>speedMIN23 /*&& frontSpeed<speedMAX23*/ && RPM>RPM23){
		shift_up_fcn();
	}
	if (frontSpeed>speedMIN34 /*&& frontSpeed<speedMAX34*/ && RPM>RPM34){
		shift_up_fcn();
	}
}


