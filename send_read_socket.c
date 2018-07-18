#include "send_read_socket.h"

#if 0
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

#endif 

int socket_init(struct message_socket *message)
{	
	int yes = 0;
	fd_set rfds;  
    struct timeval tv; 
	message->fds = rfds;
	message->tv = tv;
	
	message->sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(-1 == message->sockfd)
	{
		 printf("[MIDDLE]%s  %s  %d  socket failure\n",__FILE__,__func__,__LINE__);
		 return -1;
	}

	if (setsockopt(message->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
	{
        printf("[MIDDLE]%s  %s  %d  setsockopt failure\n",__FILE__,__func__,__LINE__);
        return -1;
    }
	
	///定义sockaddr_in
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(message->port);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		
	///bind，成功返回0，出错返回-1
	if(bind(message->sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
	{
		printf("[MIDDLE]%s  %s  %d  bind failure\n",__FILE__,__func__,__LINE__);
		return -1;
	}
	
//	printf("监听%d端口\n",MYPORT);
	///listen，成功返回0，出错返回-1
	if(listen(message->sockfd,QUEUE) == -1)
	{
		printf("[MIDDLE]%s  %s  %d  listen failure\n",__FILE__,__func__,__LINE__);
		return -1;
	}
	///客户端套接字

   return 0;
}

int socket_connect(struct message_socket *message)
{
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	
	printf("[MIDDLE]%s	%s	%d	waiting client connect......\n",__FILE__,__func__,__LINE__);
	///成功返回非负描述字，出错返回-1
	int conn = accept(message->sockfd, (struct sockaddr*)&client_addr, &length);
	if(conn<0)
	{
		perror("accept");
		return -1;
	}
	message->conn = conn;
	printf("[MIDDLE]%s	%s	%d	client connected success\n",__FILE__,__func__,__LINE__);
	
	return 0;
}

int socket_write(struct message_socket *message)
{
	if(!message)
		return -1;

	int ret = send(message->conn, message->send_data, message->send_length, 0);
//	printf("发送给客户端数据：%s\n",data);

	return ret;
}

#include "message.h"

int socket_read(struct message_socket *message)
{
	if(!message)
		return -1;

//	memset(message->get_data,0,message->get_length);
//	struct test_message *test = message->get_data;
	struct test_message *test = malloc(message->get_length);


//	int len = recv(message->sockfd, message->get_data, message->get_length,0);
	int len = recv(message->sockfd, test, message->get_length,0);

	
	printf("gets %s .............\n",test->buff);
	return len;
}

int socket_async_read(struct message_socket *message)
{
	if(!message)
		return -1;
	int len = 0;
	/* 把集合清空 */  
	FD_ZERO(&(message->fds));	

	/* 把标准输入句柄0加入到集合中 */  
	FD_SET(0, &(message->fds));  
	int maxfd = 0; 
	
	/* 把当前连接句柄new_fd加入到集合中 */	
	FD_SET(message->conn, &(message->fds));  
	if (message->conn > maxfd)	
	maxfd = message->conn;	

	/* 设置最大等待时间 */  
	message->tv.tv_sec = 1;  
	message->tv.tv_usec = 0;	

	/* 开始等待 */  
	int retval = select(maxfd + 1, &(message->fds), NULL, NULL, &(message->tv) );	
	if (retval == -1)   
	{  
		printf("将退出，select出错！ %s", strerror(errno));  
//		break;  
	}   
	else if (retval == 0)   
	{  
		/* printf("没有任何消息到来，用户也没有按键，继续等待……\n"); */  
//		continue;  
	}   
	else   
   {   
	   if (FD_ISSET(message->conn, &(message->fds)))   
	   {  
		   /* 当前连接的socket上有消息到来则接收对方发过来的消息并显示 */	
		   bzero(message->get_data, message->get_length);	
			 
		   /* 接收客户端的消息 */  
		   len = recv(message->sockfd, message->get_data, message->get_length,0);	
			 
		   if (len > 0)  
			   printf("接收消息成功:'%s'，共%d个字节的数据\n",message->get_data, len);	
		   else   
		   {  
			   if (len < 0)  
				   printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));  
			   else  
				   printf("对方退出了，聊天终止\n");	
//			   break;  
		   }  
	   }
	}

//	memset(message->get_data,0,message->get_length);
//	int len = recv(message->sockfd, message->get_data, message->get_length,0);
	return len;
}


int socket_release(struct message_socket *message)
{
	if(!message)
		return -1;
	close(message->conn);
	close(message->sockfd);	

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
	message->sockfd 			= -1;
	message->conn 				= -1;
	message->port 				= -1;
	message->send_length 		= -1;
	message->get_length 		= -1;
	message->send_data 			= NULL;
	message->get_data 			= NULL;
	
	message->init 				= socket_init;
	message->wait_connect 		= socket_connect;
	message->read 				= socket_read;
	message->write 				= socket_write;
	message->read_async 		= socket_async_read;
	message->release 			= socket_release;

	return message;
}


int message_socket_end(struct message_socket *message)
{
	if(!message)
		return -1;
	
	message->release(message);
	free(message);

	return 0;
}

