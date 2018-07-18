#include "message_parse.h"

#include "message.h"
#include "middle_to_audio.h"
#include <stdio.h>
#include <stdlib.h>

#define MIDDLE_TO_AUDIO_PORT  8887
#define MIDDLE_TO_WIFI_PORT  8888
#define MIDDLE_TO_BLUETOOTH_PORT  8889
#define MIDDLE_TO_SERVER_PORT  8891


static struct middle_to_audio_socket *server_middle_to_audio;// = middle_to_audio_socket_init();
static struct middle_to_audio_socket *server_middle_to_wifi;// = middle_to_audio_socket_init();
static struct middle_to_audio_socket *server_middle_to_bluetooth ;//= middle_to_audio_socket_init();
static struct middle_to_audio_socket *server_middle_to_sever ;//= middle_to_audio_socket_init();

int start_socket_server()
{

	server_middle_to_sever = middle_to_audio_socket_init();
	if(!server_middle_to_sever)
		return -1;	
	server_middle_to_sever->msocket->port = MIDDLE_TO_SERVER_PORT;
	int ret =  server_middle_to_sever->msocket->init(server_middle_to_sever->msocket);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	
#if 1	
	server_middle_to_audio = middle_to_audio_socket_init();
	if(!server_middle_to_audio)
		return -1;	
	server_middle_to_audio->msocket->port = MIDDLE_TO_AUDIO_PORT;
	ret =  server_middle_to_audio->msocket->init(server_middle_to_audio->msocket);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	
	server_middle_to_wifi = middle_to_audio_socket_init();
	if(!server_middle_to_wifi)
		return -1;
	
	server_middle_to_wifi->msocket->port = MIDDLE_TO_WIFI_PORT;
	ret =  server_middle_to_wifi->msocket->init(server_middle_to_wifi->msocket);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	
	server_middle_to_bluetooth = middle_to_audio_socket_init();
	if(!server_middle_to_bluetooth)
		return -1;
	
	server_middle_to_bluetooth->msocket->port = MIDDLE_TO_BLUETOOTH_PORT;
	ret =  server_middle_to_bluetooth->msocket->init(server_middle_to_bluetooth->msocket);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
#endif	
	return 0;
}

int start_conncet_server()
{
	int ret = server_middle_to_sever->msocket->wait_connect(server_middle_to_sever->msocket);

	return ret;
}

int start_conncet_audio()
{
	int ret = server_middle_to_audio->msocket->wait_connect(server_middle_to_audio->msocket);

	return ret;
}

int start_conncet_wifi()
{
	int ret = server_middle_to_wifi->msocket->wait_connect(server_middle_to_wifi->msocket);

	return ret;
}

int start_conncet_bluetooth()
{
	int ret = server_middle_to_bluetooth->msocket->wait_connect(server_middle_to_bluetooth->msocket);

	return ret;
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
			server_middle_to_sever->msocket->send_data = t_message;
			server_middle_to_sever->msocket->send_length = length;
			ret_write = server_middle_to_sever->msocket->write(server_middle_to_sever->msocket);
			printf("TO_SERVER message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_AUDIO:
			server_middle_to_audio->msocket->send_data = t_message;
			server_middle_to_audio->msocket->send_length = length;
			ret_write = server_middle_to_audio->msocket->write(server_middle_to_audio->msocket );
			printf("TO_AUDIO message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_WIFI:
			server_middle_to_wifi->msocket->send_data = t_message;
			server_middle_to_wifi->msocket->send_length = length;
			ret_write = server_middle_to_wifi->msocket->write(server_middle_to_wifi->msocket);
			printf("TO_WIFI message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_BLUETOOTH:
			server_middle_to_bluetooth->msocket->send_data = t_message;
			server_middle_to_bluetooth->msocket->send_length = length;
			ret_write = server_middle_to_bluetooth->msocket->write(server_middle_to_bluetooth->msocket );
			printf("TO_BLUETOOTH message type: %d ret = %d message:%s ret_write:%d\n",t_message->m_type,ret,t_message->buff,ret_write);
			break;
		case TO_MIDDLE:
			if(SERVER_TO_MIDDLE == t_message->m_type)
			{
				server_middle_to_sever->msocket->send_data = t_message;
				server_middle_to_sever->msocket->send_length = length;
				printf("SERVER_TO_MIDDLE message type: %d ret = %d message:%s\n",t_message->m_type,ret,t_message->buff);
				ret_write = server_middle_to_sever->msocket->write(server_middle_to_sever->msocket);
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
	server_middle_to_sever->msocket->read(server_middle_to_sever->msocket);
	server_middle_to_audio->msocket->read(server_middle_to_audio->msocket);
	server_middle_to_wifi->msocket->read(server_middle_to_wifi->msocket);
	server_middle_to_bluetooth->msocket->read(server_middle_to_bluetooth->msocket);

	return 0;
}


int message_from_server_parse(void *buff,int length)
{
	server_middle_to_sever->msocket->get_data = buff;
	server_middle_to_sever->msocket->get_length = length;
	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_sever->msocket->read(server_middle_to_sever->msocket);

	return ret;
}

int message_from_audio_parse(void *buff,int length)
{
	server_middle_to_audio->msocket->get_data = buff;
	server_middle_to_audio->msocket->get_length = length;

	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_audio->msocket->read(server_middle_to_audio->msocket);

	return ret;
}

int message_from_wifi_parse(void *buff,int length)
{
	server_middle_to_wifi->msocket->get_data = buff;
	server_middle_to_wifi->msocket->get_length = length;

	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_wifi->msocket->read(server_middle_to_wifi->msocket);

	return ret;
}

int message_from_bluetooth_parse(void *buff,int length)
{
	server_middle_to_bluetooth->msocket->get_data = buff;
	server_middle_to_bluetooth->msocket->get_length = length;

	//先接收消息然后去解析,需要采用异步机制的收
	int ret = server_middle_to_bluetooth->msocket->read(server_middle_to_bluetooth->msocket);

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

