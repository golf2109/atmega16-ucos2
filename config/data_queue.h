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

struct _DATA;
struct _DATA_BUF;
struct _DATAQUEUE;
typedef struct _DATA 		DATA;
typedef struct _DATA_BUF  	DATA_BUF;
typedef struct _DATAQUEUE 	DATAQUEUE;

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
