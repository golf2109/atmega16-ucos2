#include "app_cfg.h"

//-----------------数据结构体-----------------------
struct _DATA
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
};

struct _DATA_BUF
{
	unsigned char status;
	DATA data;
};

struct _DATAQUEUE
{
	unsigned char front;
	unsigned char rear;
	DATA data[ DATA_MAX_NUM ];
};

// ---------------定义变量----------------------
DATAQUEUE		data_queue;
DATA_BUF		data_buf;

void DataBuf_Init(void)
{
	memset( &data_buf, 0x00, sizeof(DATA_BUF) );

	data_buf.data.data_code = DATA_CODE;
	data_buf.data.data_size = DATA_MAX_SIZE;
	data_buf.data.data_end = DATA_END;
}

void DataQueue_Init(void)
{
	memset( &data_queue, 0x00, sizeof(DATAQUEUE) );
}

void DataQueue_AddData( DATA* pData )
{
	unsigned char i;

	if( IsDataQueueFull() )
		return;

	for(i=0; i<DATA_MAX_SIZE; i++)
	{
		if( pData->cDatas[i] == 0x0A )
			pData->cDatas[i] = 0x0B;
	}
	memcpy( &(data_queue.data[ data_queue.rear ]), pData, sizeof(DATA) );

	DataBuf_Init();				// 清空数据缓存
	DATAQUEUE_REAR_ADD();		// 数据个数加1
}

void DataQueue_RemoveData(void)
{
	if( IsDataQueueEmpty() )
		return;

	DATAQUEUE_FRONT_ADD();
}

DATA* GetFrontDataPrt(void)
{
	if( IsDataQueueEmpty() )
		return NULL;

	return (&data_queue.data[ data_queue.front ]);
}

bool IsDataQueueEmpty(void)
{
	return ( data_queue.rear == data_queue.front );
}

bool IsDataQueueFull(void)
{
	return ( data_queue.front == (data_queue.rear + 1) % DATA_MAX_NUM );
}
