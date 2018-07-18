#ifndef __SEND_READ_SOCKET_H_
#define __SEND_READ_SOCKET_H_

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

#include <errno.h>


#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

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

#endif
//没有进行粘包处理
struct message_socket
{
	int sockfd;
	int conn;
	int port;
	fd_set fds;
	struct timeval tv;
	int send_length;
	int get_length;
	void *send_data;
	void *get_data;
	int (*init)(struct message_socket *message);
	int (*wait_connect)(struct message_socket *message);
	int (*read)(struct message_socket *message);
	int (*read_async)(struct message_socket *message);
	int (*write)(struct message_socket *message);
	int (*release)(struct message_socket *message);
};

struct message_socket * message_socket_init();

int message_socket_end(struct message_socket *message);


#endif
