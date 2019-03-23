#ifndef CONFIG_H
#define CONFIG_H

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void TIM3_Init(void);
void TIM5_Init(void);
void MX_CAN2_Init(void);
void UARTTelemetry_Config(void);
void RxTelemetry(unsigned char c);
void USB_Config(void);
void RTC_Config(void);


#endif
