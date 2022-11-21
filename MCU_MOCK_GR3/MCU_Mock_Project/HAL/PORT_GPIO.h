#ifndef _PORT_GPIO_H_
#define _PORT_GPIO_H_

#include "MKL46Z4.h"

#define RED_LED_PIN         (1 << 29)
#define GREEN_LED_PIN       (1 << 5)


void Clock_Init();
void LED_SW_Init();
void initButton_IRQ ();
void ledInit();
void PORTC_PORTD_IRQHandler();
void delay_Ms(uint32_t Delay_CNT);
void PORT_DeInit();
void GPIO_DeInit();

#endif /* _PORT_GPIO_H_ */