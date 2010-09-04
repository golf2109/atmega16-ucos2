#ifndef __CMDQUEUE_H_
#define __CMDQUEUE_H_

#define CMD_MAX_NUM					4				//�����������֡����
#define CMD_MAX_NUM_MASK			0x03			//��������
#define PARA_MAX_SIZE				16				//����������

#define FRAME_BEGIN_FLAG 			0xAA			//֡��ʼ���

#define	CMD_RCV_BUF_STATUS_EMPTY	0x00
#define	CMD_RCV_BUF_STATUS_RCVING	0x01
#define	CMD_RCV_BUF_STATUS_READY	0x02

#define CMD_QUEUE_STATUS_FULL		1
#define CMD_QUEUE_STATUS_EMPTY		2
#define CMD_QUEUE_STATUS_HASDATA	0

#define CMDQUEUE_TAIL_ADD();		{cmd_queue.tail  = (cmd_queue.tail + 1)  & CMD_MAX_NUM_MASK;}
#define CMDQUEUE_HEAD_ADD(); 		{cmd_queue.head = (cmd_queue.head + 1) & CMD_MAX_NUM_MASK;}

typedef struct
{
	unsigned char cmd_code;						//�����������ݴ����ǣ�0x55Ϊ����
	unsigned char para_size;					//��������
	union
	{
		unsigned char para[PARA_MAX_SIZE];		//����������
		struct
		{
			unsigned char n;					
			float fPara;
		};
		struct
		{
			unsigned short int addr;
			union
			{
				float fData;
				long  lData;
				unsigned short int nData[2];
			};
		};
	};
}CMD;

typedef struct
{
	unsigned char status;
	unsigned char ptr;
	CMD cmd;
}CMD_RCV_BUF;

typedef struct
{
	unsigned char head;
	unsigned char tail;
	CMD cmd[CMD_MAX_NUM];
}CMDQUEUE;


extern CMDQUEUE		cmd_queue;
extern CMD_RCV_BUF	cmd_rcvbuf;

void cmd_rcvbuf_init(void);
void cmd_rcvbuf_add_char(unsigned char c);

void cmd_queue_init(void);
void cmd_queue_add_cmd(CMD* pCmd);
void cmd_queue_remove_cmd(void);
CMD* cmd_queue_headprt_get(void);
bool cmd_queue_status(void);

#endif
