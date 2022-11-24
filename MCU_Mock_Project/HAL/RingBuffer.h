#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include "MKL46Z4.h"

#define TRUE	(1U) 
#define FALSE 	(0U)

typedef struct
{
	int8_t Front, Rear, Size;
	uint8_t *QueueArr;
} RingQueue_Type; 

extern RingQueue_Type RxRingBuff;

void RingQueue_Init(RingQueue_Type *Queue, uint8_t* buf, uint32_t size);

uint8_t RingQueue_Full( RingQueue_Type *Queue);

uint8_t RingQueue_Empty( RingQueue_Type *Queue);

void RingQueue_Push( RingQueue_Type *Queue, uint8_t InputData);

void RingQueue_Pop( RingQueue_Type *Queue, uint8_t *OutputData);

#endif /* _RING_BUF_H_ */