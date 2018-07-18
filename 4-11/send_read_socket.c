#include "send_read_socket.h"

int socket_init(int *sockfd,int *connfd,int port)
{
	///定义sockfd
	int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
	*sockfd = server_sockfd;

	
	///定义sockaddr_in
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(port);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		
	///bind，成功返回0，出错返回-1
	if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
	{
		perror("bind");
		return -1;
	}
	
//	printf("监听%d端口\n",MYPORT);
	///listen，成功返回0，出错返回-1
	if(listen(server_sockfd,QUEUE) == -1)
	{
		perror("listen");
		return -1;
	}
	///客户端套接字

   struct sockaddr_in client_addr;
   socklen_t length = sizeof(client_addr);
   
   printf("等待客户端连接\n");
   ///成功返回非负描述字，出错返回-1
   int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
   if(conn<0)
   {
	   perror("connect");
	   return -1;
   }
   *connfd = conn;
   printf("客户端成功连接\n");

   return 0;
}

int socket_write(int conn,void *data,int data_length)
{
	int ret = send(conn, data, data_length, 0);
//	printf("发送给客户端数据：%s\n",data);

	return ret;
}

int socket_read(int conn,void *data,int data_length)
{
	memset(data,0,data_length);
	int len = recv(conn, data, data_length,0);
	//客户端发送exit或者异常结束时，退出
	if(strcmp(data,"exit\n")==0 || len<=0)
		return -1;

	return len;
}

int socket_release(int sockfd,int conn)
{
	close(conn);
	close(sockfd);	

	return 0;
}

struct message_socket *message_socket_init()
{
	struct message_socket *message = (struct message_socket *)malloc(sizeof(struct message_socket));
	if(!message)
		return NULL;
#if 0	
	int ret = message_socket_init(&message->sockfd,&message->conn);
	if(ret < 0)
		return -1;
#endif
	message->sockfd = -1;
	message->conn = -1;
	message->port = -1;
	
	message->init = socket_init;
	message->read = socket_read;
	message->write = socket_write;
	message->release = socket_release;

	return message;
}


int message_socket_end(struct message_socket *message)
{
	if(!message)
		return -1;
	
	message->release(message->sockfd,message->conn);
	free(message);

	return 0;
}

