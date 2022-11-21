#include "ADC.h"



void ADC_Init()
{
    // clock ADC, PORT
  SIM->SCGC6 |= SIM_SCGC6_ADC0(1); 
  SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
  
  // select bus clock for ADC
  ADC0->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
  
  // ADIV = 1
  
  // config Mode port (analog)
  PORTE->PCR[22] &= ~PORT_PCR_MUX_MASK;
  
  // set up mode ADC
  // select Chanel [0]; Single-ended conversions
  ADC0->SC1[0] &= ~ADC_SC1_DIFF_MASK;
  
  // Conversion mode selection 
  ADC0->CFG1 |= ADC_CFG1_MODE(3);  // 16 Bit Conversion
  
  // Sample Time Configuration 
  ADC0->CFG1 |= ADC_CFG1_ADLSMP(1); // Short sample time
  
}

uint16_t ADC_Read()
{
  
  ADC_Start();
  
  // Wait Conversion Complete Flag
  while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)); 
  
  // Read ADC
  return ADC0->R[0]; 
}

void ADC_SetupINT()
{
    //enable ADC interrup
    ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
    
    // enable NIVI interrup ADC
    NVIC_EnableIRQ(ADC0_IRQn); //15
}

void ADC_Start()
{
    // start Conversion
  ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK; // Clear Input channel select
  ADC0->SC1[0] |= ADC_SC1_ADCH(3); // select input chanel DADP3
  
}