#include "config.h"

// 定义变量
CMDQUEUE		cmd_queue;
CMD_RCV_BUF		cmd_rcv_buf;

void CmdRcvBuf_Init(void)
{
	memset(&cmd_rcv_buf, 0x00, sizeof(CMD_RCV_BUF));
}

void CmdRcvBuf_AddChar(unsigned char c)
{
	if(CMD_RCV_BUF_STATUS_EMPTY == cmd_rcv_buf.status)
	{
		if(c == FRAME_BEGIN_FLAG)
		{
			cmd_rcv_buf.status = CMD_RCV_BUF_STATUS_RCVING;
			cmd_rcv_buf.ptr = 0;
		}

		return;
	}

	((unsigned char *)&cmd_rcv_buf.cmd)[ cmd_rcv_buf.ptr ++ ] = c;
	if( (cmd_rcv_buf.ptr >= 2) && 
		(cmd_rcv_buf.ptr == cmd_rcv_buf.cmd.para_size + 2 ) )
	{
		cmd_rcv_buf.status = CMD_RCV_BUF_STATUS_READY;
	}

	if( cmd_rcv_buf.status == CMD_RCV_BUF_STATUS_READY )
	{
		CmdQueue_AddCmd( &cmd_rcv_buf.cmd );
		cmd_rcv_buf.status = CMD_RCV_BUF_STATUS_EMPTY;
		cmd_rcv_buf.ptr = 0;
	}
}

void CmdQueue_Init(void)
{
	memset( &cmd_queue, 0x00, sizeof(CMDQUEUE) );
}

void CmdQueue_AddCmd( CMD* pCmd )
{
	if( IsCmdQueueFull() )
		return;

	memcpy( &(cmd_queue.cmd[ cmd_queue.rear ]), pCmd, sizeof(CMD) );
	CMDQUEUE_REAR_ADD();
}

void CmdQueue_RemoveCmd(void)
{
	if( IsCmdQueueEmpty() )
		return;

	CMDQUEUE_FRONT_ADD();
}

CMD* GetFrontCmdPrt(void)
{
	if( IsCmdQueueEmpty() )
		return NULL;

	return (&cmd_queue.cmd[ cmd_queue.front ]);
}

bool IsCmdQueueEmpty(void)
{
	return ( cmd_queue.rear == cmd_queue.front );
}

bool IsCmdQueueFull(void)
{
	return ( cmd_queue.front == (cmd_queue.rear + 1) % CMD_MAX_NUM );
}
