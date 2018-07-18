#include "message_parse.h"

#include "message.h"
#include "middle_to_audio.h"
#include <stdio.h>
#include <stdlib.h>

#define MIDDLE_TO_AUDIO_PORT  8887
#define MIDDLE_TO_WIFI_PORT  8888
#define MIDDLE_TO_BLUETOOTH_PORT  8889
#define MIDDLE_TO_SERVER_PORT  8891

#if 0
struct message_socket
{
	int sockfd;
	int conn;
	int port;
	int (*init)(int *sockfd,int *connfd,int port);
	int (*read)(int conn,void *data,int data_length);
	int (*write)(int conn,void *data,int data_length);
	int (*release)(int sockfd,int conn);
};

struct middle_to_audio_socket
{
	struct message_socket *msocket;
	int port;
};
#endif

static struct middle_to_audio_socket *server_middle_to_audio;// = middle_to_audio_socket_init();
static struct middle_to_audio_socket *server_middle_to_wifi;// = middle_to_audio_socket_init();
static struct middle_to_audio_socket *server_middle_to_bluetooth ;//= middle_to_audio_socket_init();
static struct middle_to_audio_socket *server_middle_to_sever ;//= middle_to_audio_socket_init();

int start_socket_server()
{

	server_middle_to_sever = middle_to_audio_socket_init();
	if(!server_middle_to_sever)
		return -1;	
	server_middle_to_sever->port = MIDDLE_TO_SERVER_PORT;
	int ret =  server_middle_to_sever->msocket->init(&server_middle_to_sever->msocket->sockfd,&server_middle_to_sever->msocket->conn,server_middle_to_sever->port);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	
#if 1	
	server_middle_to_audio = middle_to_audio_socket_init();
	if(!server_middle_to_audio)
		return -1;	
	server_middle_to_audio->port = MIDDLE_TO_AUDIO_PORT;
	ret =  server_middle_to_audio->msocket->init(&server_middle_to_audio->msocket->sockfd,&server_middle_to_audio->msocket->conn,server_middle_to_audio->port);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	
	server_middle_to_wifi = middle_to_audio_socket_init();
	if(!server_middle_to_wifi)
		return -1;
	
	server_middle_to_wifi->port = MIDDLE_TO_WIFI_PORT;
	ret =  server_middle_to_wifi->msocket->init(&server_middle_to_wifi->msocket->sockfd,&server_middle_to_wifi->msocket->conn,server_middle_to_wifi->port);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	
	server_middle_to_bluetooth = middle_to_audio_socket_init();
	if(!server_middle_to_bluetooth)
		return -1;
	
	server_middle_to_bluetooth->port = MIDDLE_TO_BLUETOOTH_PORT;
	ret =  server_middle_to_bluetooth->msocket->init(&server_middle_to_bluetooth->msocket->sockfd,&server_middle_to_bluetooth->msocket->conn,server_middle_to_bluetooth->port);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
#endif	
	return 0;
}




/* 解析消息是属于哪一类消息 */
int base_message_parse(const void *message)
{
	struct test_message *t_message = (struct test_message *)message;
	if((MIDDLE_TO_SERVER == t_message->m_type) | (AUDIO_TO_SERVER == t_message->m_type) | (WIFI_TO_SERVER == t_message->m_type) | (BLUETOOTH_TO_SERVER == t_message->m_type))
		return TO_SERVER;
	else if((MIDDLE_TO_AUDIO == t_message->m_type) | (BLUETOOTH_TO_AUDIO == t_message->m_type) | (WIFI_TO_AUDIO == t_message->m_type) | (SERVER_TO_AUDIO == t_message->m_type))
		return TO_AUDIO;
	else if((MIDDLE_TO_WIFI == t_message->m_type) | (BLUETOOTH_TO_WIFI == t_message->m_type) | (AUDIO_TO_WIFI == t_message->m_type) | (SERVER_TO_WIFI == t_message->m_type))
		return TO_WIFI;
	else if((MIDDLE_TO_BLUETOOTH == t_message->m_type) | (AUDIO_TO_BLUETOOTH == t_message->m_type) | (WIFI_TO_BLUETOOTH == t_message->m_type) | (SERVER_TO_BLUETOOTH == t_message->m_type))
	{
		return TO_BLUETOOTH;
	}
	else if((SERVER_TO_MIDDLE == t_message->m_type) | (AUDIO_TO_MIDDLE == t_message->m_type) | (WIFI_TO_MIDDLE == t_message->m_type) | (BLUETOOTH_TO_MIDDLE == t_message->m_type))
	{
		return TO_MIDDLE;
	}
	else 
		return INVALID_TYPE;

}


void check_sockfd()
{
	if(-1 == server_middle_to_sever->msocket->sockfd)
	{
		printf("will wait to server socket connect....\n");
		while(-1 == server_middle_to_sever->msocket->sockfd)
		{
			;
		}
	}
	else if(-1 == server_middle_to_audio->msocket->sockfd)
	{
		printf("will wait to audio socket connect....\n");
		while(-1 == server_middle_to_audio->msocket->sockfd)
		{
			;
		}
	}
	else if(-1 == server_middle_to_wifi->msocket->sockfd)
	{
		printf("will wait to wifi socket connect....\n");
		while(-1 == server_middle_to_wifi->msocket->sockfd)
		{
			;
		}
	}
	else if(-1 == server_middle_to_bluetooth->msocket->sockfd)
	{
		printf("will wait to bluetooth socket connect....\n");
		while(-1 == server_middle_to_bluetooth->msocket->sockfd)
		{
			;
		}
	}
}


/* 解析收到的消息,然后判断是需要发送给哪一个模块的 */
int message_parse(const void *message,int length)
{
//	check_sockfd();
	struct test_message *t_message = (struct test_message *)message;	
	int ret = base_message_parse(message);
//	printf("message type: %d ret = %d message:%s\n",t_message->m_type,ret,t_message->buff);
	
	int ret_write = -1;
	switch(ret)
	{
		case TO_SERVER:
			ret_write = server_middle_to_sever->msocket->write(server_middle_to_sever->msocket->conn,message,length);
			printf("TO_SERVER message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_AUDIO:
			ret_write = server_middle_to_audio->msocket->write(server_middle_to_audio->msocket->conn,message,length);
			printf("TO_AUDIO message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_WIFI:
			ret_write = server_middle_to_wifi->msocket->write(server_middle_to_wifi->msocket->conn,message,length);
			printf("TO_WIFI message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_BLUETOOTH:
			ret_write = server_middle_to_bluetooth->msocket->write(server_middle_to_bluetooth->msocket->conn,message,length);
			printf("TO_BLUETOOTH message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_MIDDLE:
			if(SERVER_TO_MIDDLE == t_message->m_type)
			{
				printf("SERVER_TO_MIDDLE message type: %d ret = %d message:%s\n",t_message->m_type,ret,t_message->buff);
				ret_write = server_middle_to_sever->msocket->write(server_middle_to_sever->msocket->conn,message,length);
				printf("ret_write=%d t_message:%s length=%d t_message->length =%d \n",ret_write,t_message->buff,length,t_message->length);
			}
			break;
		default:
			printf("This type is not recongized %d!\n",ret);
			break;

	}

	return ret_write;
}

/* 解析从各个模块收到的消息，然后判断是需要发送给哪一个模块的 */
int message_from_model_parse(const void *message,int length)
{
	//如果解析方式一样就用上面的函数就可以

	void *buff = (char *)malloc(length);
	//先接收消息然后去解析,需要采用异步机制的收
	server_middle_to_sever->msocket->read(server_middle_to_sever->msocket->conn,buff,length);
	server_middle_to_audio->msocket->read(server_middle_to_audio->msocket->conn,buff,length);
	server_middle_to_wifi->msocket->read(server_middle_to_wifi->msocket->conn,buff,length);
	server_middle_to_bluetooth->msocket->read(server_middle_to_bluetooth->msocket->conn,buff,length);

	return 0;
}


int message_from_server_parse(void *buff,int length)
{
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_sever->msocket->read(server_middle_to_sever->msocket->conn,buff,length);

	return ret;
}

int message_from_audio_parse(void *buff,int length)
{
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_audio->msocket->read(server_middle_to_audio->msocket->conn,buff,length);

	return ret;
}

int message_from_wifi_parse(void *buff,int length)
{
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_wifi->msocket->read(server_middle_to_wifi->msocket->conn,buff,length);

	return ret;
}

int message_from_bluetooth_parse(void *buff,int length)
{
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_bluetooth->msocket->read(server_middle_to_bluetooth->msocket->conn,buff,length);

	return ret;
}

#if 0
int message_from_server_parse()
{
	int length = 1024;
	void *buff = (char *)malloc(length);
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_sever->msocket->read(server_middle_to_sever->msocket->conn,buff,length);

	struct test_message* print = (struct test_message*)buff;	
	printf("get message from pid %d  is %s,ret=%d \n",print->pid ,print->buff,ret);

	sleep(5);
//	message_parse(buff, length);
	message_parse(buff, ret);


	return 0;
}

int message_from_audio_parse()
{
	int length = 1024;
	void *buff = (char *)malloc(length);
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_audio->msocket->read(server_middle_to_audio->msocket->conn,buff,length);

	struct test_message* print = (struct test_message*)buff;	
	printf("get message from pid %d  is %s,ret=%d type is %d\n",print->pid ,print->buff,ret,print->m_type);

	message_parse(buff, length);

	return 0;

}

int message_from_wifi_parse()
{
	int length = 1024;
	void *buff = (char *)malloc(length);
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_wifi->msocket->read(server_middle_to_wifi->msocket->conn,buff,length);

	struct test_message* print = (struct test_message*)buff;	
	printf("get message from pid %d  is %s,ret=%d \n",print->pid ,print->buff,ret);

	message_parse(buff, length);

	return 0;

}

int message_from_bluetooth_parse()
{
	int length = 1024;
	void *buff = (char *)malloc(length);
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_bluetooth->msocket->read(server_middle_to_bluetooth->msocket->conn,buff,length);

	struct test_message* print = (struct test_message*)buff;	
	printf("get message from pid %d  is %s,ret=%d \n",print->pid ,print->buff,ret);

	message_parse(buff, length);

	return 0;

}

#endif



int end_socket_server()
{
	middle_to_audio_socket_end(server_middle_to_audio);
	middle_to_audio_socket_end(server_middle_to_wifi);
	middle_to_audio_socket_end(server_middle_to_bluetooth);
	middle_to_audio_socket_end(server_middle_to_sever);

	return 0;
}

