#ifndef _UART0_H_
#define _UART0_H_

#include "MKL46Z4.h"

#define UART0_RX_PIN    1
#define UART0_TX_PIN    2
#define BAUD_RATE       9600U

void UART0_Clock2Mhz();

void UART0_Clock48Mhz();

void UART0_PortInit();

void UART0_SetBaudRate(uint32_t clockSource, uint32_t baudRate);

void UART0_SetFrame();

void UART0_Init();

void UART0_TransmitBuffer(uint8_t * buffer, uint32_t len);

void UART0_ReceiveBuffer(uint8_t * buffer, uint32_t len);

void UART0_TransmitData(uint8_t u8Data);

uint8_t UART0_ReceiveData(void);

void UART0_SetupReceiveINT();

void UART0_DeInit();

void Clock_DeInit();

#endif /* _UART0_H_ */