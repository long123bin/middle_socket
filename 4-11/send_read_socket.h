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

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

#if 0
struct message_socket
{
	int sockfd,
	int conn;
	int message_socket_init(int *sockfd);
	int message_socket_read(int conn,void *data,int data_length);
	int message_socket_write(int conn,void *data,int data_length);
	int message_socket_release(int sockfd,int conn);
};
#endif

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

struct message_socket * message_socket_init();

int message_socket_end(struct message_socket *message);


#endif
