#ifndef __CMDQUEUE_H_
#define __CMDQUEUE_H_

#define CMD_MAX_NUM					4				//命令缓冲队列最大帧个数
#define CMD_MAX_NUM_MASK			0x03			//命令掩码
#define PARA_MAX_SIZE				16				//最大参数长度

#define FRAME_BEGIN_FLAG 			0xAA			//帧起始标记

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
	unsigned char cmd_code;						//命令代码或数据代码标记，0x55为数据
	unsigned char para_size;					//参数长度
	union
	{
		unsigned char para[PARA_MAX_SIZE];		//参数缓冲区
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
