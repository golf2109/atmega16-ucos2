#include "config.h"

// �������
CMDQUEUE		cmd_queue;
CMD_RCV_BUF		cmd_rcvbuf;

void cmd_rcvbuf_init(void)								//������ջ�������ʼ��
{
	memset(&cmd_rcvbuf, 0x00, sizeof(CMD_RCV_BUF));
}

void cmd_rcvbuf_add_char(unsigned char c)				//��������ջ��������һ���ַ�
{
	if(CMD_RCV_BUF_STATUS_EMPTY == cmd_rcvbuf.status)	//���������ջ������ǿյ�
	{
		if(c == FRAME_BEGIN_FLAG)						//������յ����ַ�Ϊ֡��ʼ���0XAA
		{
			cmd_rcvbuf.status = CMD_RCV_BUF_STATUS_RCVING;//��ô�ͱ�����������ǰ״̬Ϊ���ڽ���״̬
			cmd_rcvbuf.ptr 	  = 0;						//����ָ������Ϊ0
		}

		return;											//���أ���ʱ�ٽ��յ����ݾ�ֱ��ִ�����²��֣����������������
	}

	((unsigned char *)&cmd_rcvbuf.cmd)[cmd_rcvbuf.ptr++] = c;//�����յ����ַ��ŵ����ջ�������
	
	if((cmd_rcvbuf.ptr >= 2) && (cmd_rcvbuf.ptr == cmd_rcvbuf.cmd.para_size + 2))//ǰ�����ַ�Ϊ���������ݱ�ǺͲ������ȣ���Ҫ����2
	{
		cmd_rcvbuf.status = CMD_RCV_BUF_STATUS_READY;	//ÿ֡����ĳ���Ҳ��ȷ���ģ�������һ���ж�
	}

	if(cmd_rcvbuf.status == CMD_RCV_BUF_STATUS_READY)	//���һ֡�����Ѿ�������ɣ���������ջ������Ѿ���
	{
		cmd_queue_add_cmd(&cmd_rcvbuf.cmd);				//�򽫵�ǰ������ӵ�������ն��л�������
		cmd_rcvbuf.status = CMD_RCV_BUF_STATUS_EMPTY;	//������Ӧ���������״̬Ϊ��
		cmd_rcvbuf.ptr = 0;								//��ȡָ��Ϊ0
	}
}

void cmd_queue_init(void)								//�������г�ʼ��
{
	memset(&cmd_queue, 0x00, sizeof(CMDQUEUE));
}

void cmd_queue_add_cmd(CMD* pCmd)						//�������������һ֡����
{
	if(cmd_queue_status() == CMD_QUEUE_STATUS_FULL)		//�����������������ֱ�ӷ���
		return;

	memcpy(&(cmd_queue.cmd[cmd_queue.tail]), pCmd, sizeof(CMD));//��pCmdָ�������Ƶ�������е�β��
	CMDQUEUE_TAIL_ADD();								//����β��ָ��
}

void cmd_queue_remove_cmd(void)							//���������У�ͷ������һ֡����
{
	if(cmd_queue_status() == CMD_QUEUE_STATUS_EMPTY)
		return;

	CMDQUEUE_HEAD_ADD();								//һ�����ݾ��Ǵ�β����ӣ���ͷ��ɾ������ͷ����ȡ��
}

CMD* cmd_queue_headprt_get(void)						//��ȡ�������е�ͷָ��
{
	if(cmd_queue_status() == CMD_QUEUE_STATUS_EMPTY)
		return NULL;

	return (&cmd_queue.cmd[cmd_queue.head]);
}

bool cmd_queue_status(void)								//���ص�ǰ�������е�״̬���գ�������������
{
	if(cmd_queue.tail == cmd_queue.head)
		return CMD_QUEUE_STATUS_EMPTY;
	
	if(cmd_queue.head == (cmd_queue.tail + 1) % CMD_MAX_NUM)
		return CMD_QUEUE_STATUS_FULL;
	
	else
		return CMD_QUEUE_STATUS_HASDATA;
}
