#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include "message.h"

#define MYPORT  8887
#define BUFFER_SIZE 1024
char* SERVER_IP = "127.0.0.1";

int main(int argc,char **argv)
{
	struct test_message test;
//	test.test_data = (char *)malloc(1024);
	int length = sizeof(struct test_message);
	test.length = length;
	test.pid = getpid();

	int turn = 0;
	
	
	enum Messgae_type type;

    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

	int port = 0;
	if(argc < 2)
	{
		port = MYPORT;
		servaddr.sin_port = htons(port);  ///服务器端口
	}
    else
	{
		port = atoi(argv[1]);
		servaddr.sin_port = htons(port);  ///服务器端口
	}
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);  ///服务器ip

	if(8891 == port)
	{
		printf("This modle is server\n");
		type = SERVER;
	}
    else if(8887 == port)
	{
		printf("This modle is audio\n");
		type = AUDIO;
	}
	else if(8888 == port)
	{
		printf("This modle is WIFI\n");
		type = WIFI;
	}
	else if(8889 == port)
	{
		printf("This modle is bluetooth\n");
		type = BLUETOOTH;
	}
	else
	{
		printf("INVALID_TYPE \n");
		type = INVALID_TYPE;
		return -1;
	}
	
    printf("连接%s:%d\n",SERVER_IP,port);
    ///连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    printf("服务器连接成功\n");
//    char sendbuf[BUFFER_SIZE];
//    char recvbuf[BUFFER_SIZE];
	while(1)
	{
		
/*
 * turn为1时:
 *		1:server模块往audio模块发送数据;2:audio模块往wifi模块发送数据;3:wifi模块往bluetooth模块发送数据;4:bluetooth模块往server模块发送数据;
 * turn为2时:
 *		1:server模块往wifi模块发送数据;2:audio模块往bluetooth模块发送数据;3:wifi模块往server模块发送数据;4:bluetooth模块往audio模块发送数据;
 * turn为3时:
 *		1:server模块往bluetooth模块发送数据;2:audio模块往server模块发送数据;3:wifi模块往audio模块发送数据;4:bluetooth模块往wifi模块发送数据;
 * turn为4时:
 *		1:server模块往audio模块发送数据;2:audio模块往wifi模块发送数据;3:wifi模块往bluetooth模块发送数据;4:bluetooth模块往server模块发送数据;
 */
 		memset(test.buff,0,128);
		turn ++;
		if(1 == turn)
		{
			switch (type)
			{
				case SERVER:
					test.m_type = SERVER_TO_AUDIO;
					sprintf(test.buff,"server to audio modle port %d",port);
					break;
				case AUDIO:
					test.m_type = AUDIO_TO_WIFI;
					sprintf(test.buff,"audio to wifi modle port %d",port);
					break;
				case WIFI:
					test.m_type = WIFI_TO_BLUETOOTH;
					sprintf(test.buff,"wifi to bluetooth modle port %d",port);
					break;
				case BLUETOOTH:
					test.m_type = BLUETOOTH_TO_SERVER;
					sprintf(test.buff,"bluetooth to server modle port %d",port);
					break;
				default:
					break;
			}
		}
		else if(2 == turn)
		{
			switch (type)
			{
				case SERVER:
					test.m_type = SERVER_TO_WIFI;
					sprintf(test.buff,"server to wifi modle port %d",port);
					break;
				case AUDIO:
					test.m_type = AUDIO_TO_BLUETOOTH;
					sprintf(test.buff,"audio to bluetooth modle port %d",port);
					break;
				case WIFI:
					test.m_type = WIFI_TO_SERVER;
					sprintf(test.buff,"WIFI to server modle port %d",port);
					break;
				case BLUETOOTH:
					test.m_type = BLUETOOTH_TO_AUDIO;
					sprintf(test.buff,"bluetooth to audio modle port %d",port);
					break;
				default:
					break;
			}

		}
		else if(3 == turn)
		{
			turn = 0;
			switch (type)
			{
				case SERVER:
					test.m_type = SERVER_TO_BLUETOOTH;
					sprintf(test.buff,"server to bluetooth modle port %d",port);
					break;
				case AUDIO:
					test.m_type = AUDIO_TO_SERVER;
					sprintf(test.buff,"audio to server modle port %d",port);
					break;
				case WIFI:
					test.m_type = WIFI_TO_AUDIO;
					sprintf(test.buff,"WIFI to audio modle port %d",port);
					break;
				case BLUETOOTH:
					test.m_type = BLUETOOTH_TO_WIFI;
					sprintf(test.buff,"bluetooth to wifi modle port %d",port);
					break;
				default:
					break;
			}

		}
		else
			printf("to extern \n");
#if 0
		memset(test.buff,0,128);
		if(type == SERVER)
		{
			test.m_type = SERVER_TO_AUDIO;
			strcpy(test.buff,"This is socket test SERVER_TO_AUDIO");
		}
		else if(type == AUDIO)
		{
			test.m_type = AUDIO_TO_SERVER;
			strcpy(test.buff,"This is socket test AUDIO_TO_SERVER");
		}
		test.m_type = SERVER_TO_MIDDLE;
		test.port = port;
		sprintf(test.buff,"This is socket test SERVER_TO_MIDDLE %d",test.port);
#endif		
		printf("向服务器发送数据：%s.....length:%d..sizeof(test):%d....%d test.m_type:%d port:%d\n",test.buff,length,sizeof(test),test.pid,test.m_type,port);
		int rets = send(sock_cli,(char *) &test, sizeof(test),0); ///发送

		memset(test.buff,0,128);
		test.pid = 0;
		
        int retr = recv(sock_cli, (char *)&test, sizeof(test),0); ///接收
		printf("从服务器接收数据：%s...rets:%d..retr:%d..%d\n\n",test.buff,rets,retr,test.pid);
	}

#if 0	
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        printf("向服务器发送数据：%s\n",sendbuf);
        send(sock_cli, sendbuf, strlen(sendbuf),0); ///发送
        if(strcmp(sendbuf,"exit\n")==0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
        printf("从服务器接收数据：%s\n",recvbuf);
        
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
#endif    
    close(sock_cli);
    return 0;
}

