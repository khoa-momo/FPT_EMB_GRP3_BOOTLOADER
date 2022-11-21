#ifndef _ADC_H_
#define _ADC_H_

#include "MKL46Z4.h"

/* Innit ADC */
void ADC_Init();

/* Read ADC */
uint16_t ADC_Read();

void ADC_SetupINT();

void ADC_Start();

#endif /* _ADC_H_ */