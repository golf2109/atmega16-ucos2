#ifndef __DATAQUEUE_H_
#define __DATAQUEUE_H_

#define DATA_MAX_NUM				4
#define DATA_MAX_NUM_MASK			0x03
#define DATA_MAX_SIZE				24
#define DATA_LENGTH					(DATA_MAX_SIZE+3)

#define	DATA_CODE					0x55
#define	DATA_END					0x0A

#define DATAQUEUE_REAR_ADD();		{data_queue.rear  = (data_queue.rear + 1)  & DATA_MAX_NUM_MASK;}
#define DATAQUEUE_FRONT_ADD();	 	{data_queue.front = (data_queue.front + 1) & DATA_MAX_NUM_MASK;}

//-----------------数据结构体-----------------------
typedef struct
{
	unsigned char data_code;
	unsigned char data_size;
	union{
		unsigned char cDatas[ DATA_MAX_SIZE ];
		float fDatas[ DATA_MAX_SIZE/4 ];
		struct{
			float fCurrent;
			float fVoltage;
			float fPower;
			float fFlowSpeed;
			float fTorque;
			float fRotateSpeed;
		};
	};
	unsigned char data_end;
}DATA;

typedef struct
{
	unsigned char status;
	DATA data;
}DATA_BUF;

typedef struct
{
	unsigned char front;
	unsigned char rear;
	DATA data[ DATA_MAX_NUM ];
}DATAQUEUE;


extern DATAQUEUE			data_queue;
extern DATA_BUF				data_buf;

void DataBuf_Init(void);
void DataQueue_Init(void);
void DataQueue_AddData(DATA* pData);
void DataQueue_RemoveData(void);
DATA* GetFrontDataPrt(void);
bool IsDataQueueEmpty(void);
bool IsDataQueueFull(void);

#endif
