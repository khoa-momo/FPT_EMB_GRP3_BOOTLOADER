#include "RingBuffer.h" 

void RingQueue_Init(RingQueue_Type *Queue, uint8_t* buf, uint32_t size)
{
	Queue->QueueArr = buf;
	Queue->Front = -1;
	Queue->Rear = -1;
	Queue->Size = size;
}

uint8_t RingQueue_Full( RingQueue_Type *Queue)
{
	uint8_t Key = FALSE; 
	if((Queue->Front == Queue->Rear + 1) || (Queue->Front == 0 && Queue->Rear == Queue->Size - 1))
	{
		Key = TRUE;
	}
	return Key;
}

uint8_t RingQueue_Empty( RingQueue_Type *Queue)
{
	uint8_t Key = FALSE; 
	if(Queue->Front == -1)
	{
		Key = TRUE;
	}
	return Key;
}

void RingQueue_Push( RingQueue_Type *Queue, uint8_t InputData)
{
	if (!RingQueue_Full(Queue))
	{
            if (Queue->Front == -1)
            {
                Queue->Front = 0;
            }
            
            Queue->Rear = (Queue->Rear + 1) % Queue->Size;
            
            Queue->QueueArr[Queue->Rear] = InputData;
            
	}
        
}

void RingQueue_Pop( RingQueue_Type *Queue , uint8_t *OutputData)
{	 
	if (!RingQueue_Empty(Queue))
	{
            *OutputData = Queue->QueueArr[Queue->Front]; 
            if (Queue->Front == Queue->Rear)
            {
                Queue->Front = - 1;
                Queue->Rear = - 1;
            }
            else
            {
                Queue->Front = (Queue->Front + 1) % Queue->Size;           
            }
	}
}
