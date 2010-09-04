#ifndef __DATAQUEUE_H_
#define __DATAQUEUE_H_

#define DATA_MAX_NUM				4
#define DATA_MAX_NUM_MASK			0x03
#define DATA_MAX_SIZE				24
#define DATA_LENGTH					(DATA_MAX_SIZE+3)

#define	DATA_CODE					0x55
#define	DATA_END					0x0A

#define DATAQUEUE_STATUS_FULL		1
#define DATAQUEUE_STATUS_EMPTY		2
#define DATAQUEUE_STATUS_HASDATA	0

#define DATAQUEUE_TAIL_ADD();		{data_queue.tail = (data_queue.tail + 1) & DATA_MAX_NUM_MASK;} //��β�����һ֡
#define DATAQUEUE_FRONT_ADD();	 	{data_queue.head = (data_queue.head + 1) & DATA_MAX_NUM_MASK;} //������ȥ��ͷ��һ֡

//-----------------���ݽṹ��-----------------------
typedef struct
{
	unsigned char data_code;			//���ݻ�������룬0x55��ʾ���ݣ�����Ϊ����
	unsigned char data_size;			//���ݳ���
	union
	{
		unsigned char cDatas[DATA_MAX_SIZE];//���ݻ�����
		float fDatas[DATA_MAX_SIZE/4];	//���ڸ�����������������6��������������δ�õ�
		struct							//���½ṹ��Ҳδ�õ�~����
		{
			float fCurrent;				//����
			float fVoltage;				//��ѹ
			float fPower;				//����
			float fFlowSpeed;			//����
			float fTorque;				//ת��
			float fRotateSpeed;			//ת��
		};
	};
	unsigned char data_end;				//����β
}DATA;

typedef struct
{
	unsigned char status;				//״̬
	DATA data;							//һ֡����
}DATA_BUF;

typedef struct
{
	unsigned char head;				//���ݶ���ͷ
	unsigned char tail;					//���ݶ���β
	DATA data[DATA_MAX_NUM];			//����Ԫ�ػ���������DATA_MAX_NUM֡����
}DATAQUEUE;


extern DATAQUEUE			data_queue;
extern DATA_BUF				data_buf;

void data_buf_init(void);
void data_queue_init(void);
void data_queue_add_data(DATA* pData);
void data_queue_remove_data(void);
DATA* data_queue_headptr_get(void);
bool data_queue_status(void);

#endif
