#ifndef __CMDQUEUE_H_
#define __CMDQUEUE_H_

#define CMD_MAX_NUM					4
#define CMD_MAX_NUM_MASK			0x03
#define PARA_MAX_SIZE				16

#define FRAME_BEGIN_FLAG 			0xAA

#define	CMD_RCV_BUF_STATUS_EMPTY	0x00
#define	CMD_RCV_BUF_STATUS_RCVING	0x01
#define	CMD_RCV_BUF_STATUS_READY	0x02

#define CMDQUEUE_REAR_ADD();		{cmd_queue.rear = (cmd_queue.rear + 1) & CMD_MAX_NUM_MASK;}
#define CMDQUEUE_FRONT_ADD(); 		{cmd_queue.front = (cmd_queue.front + 1) & CMD_MAX_NUM_MASK;}

typedef struct
{
	unsigned char cmd_code;
	unsigned char para_size;
	union
	{
		unsigned char para[ PARA_MAX_SIZE ];
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
	unsigned char front;
	unsigned char rear;
	CMD cmd[ CMD_MAX_NUM ];
}CMDQUEUE;


extern CMDQUEUE		cmd_queue;
extern CMD_RCV_BUF	cmd_rcv_buf;

extern void CmdRcvBuf_Init(void);
extern void CmdRcvBuf_AddChar(unsigned char c);

extern void CmdQueue_Init(void);
extern void CmdQueue_AddCmd(CMD* pCmd);
extern void CmdQueue_RemoveCmd(void);
extern CMD* GetFrontCmdPrt(void);
extern bool IsCmdQueueEmpty(void);
extern bool IsCmdQueueFull(void);

#endif
