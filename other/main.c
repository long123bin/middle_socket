#include "send_read_socket.h"
#include "message.h"

#if 0
int main(int argc,char **argv)
{
	struct test_message test;
//	test.test_data = (char *)malloc(1024);
//	if(!test.test_data)
//	{
//		printf("malloc failure\n");
//		perror("malloc");

//		return -1;
//	}
	
	int length = sizeof(struct test_message);
	
//	char buff[1024] = {0};
	struct message_socket *msocket = message_socket_init();
	int ret =  msocket->init(&msocket->sockfd,&msocket->conn,-1);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	while(1)
	{
//		memset(test.test_data,0,1024);
		memset(test.buff,0,128);
//		msocket->read(msocket->conn, &test, length);
		msocket->read(msocket->conn, (char *)&test, sizeof(test));

		printf("get message from pid %d \n",test.pid );
//		if(!test.test_data)
//		{
//			printf("get data is NULL\n");
//			continue;
//		}
		printf("get  message is %s\n", test.buff);

		test.pid = getpid();
//		memset(test.test_data,0,1024);
		memset(test.buff,0,128);
		strcpy(test.buff,"This is socket test buff from main");
//		strcpy(test.test_data,"This is socket test for main");
		sleep(2);
//		msocket->write(msocket->conn, &test, length);
		msocket->write(msocket->conn, (char *)&test, sizeof(test));
	}

	message_socket_end(msocket);	
	return 0;
}
#endif

//#include "middle_to_audio.h"
#if 0
int main(int argc, char ** argv)
{
	struct test_message test;

	
	int length = sizeof(struct test_message);
	
	struct middle_to_audio_socket *mtasocket = middle_to_audio_socket_init();
	if(!mtasocket)
		return -1;
	int ret =  mtasocket->msocket->init(&mtasocket->msocket->sockfd,&mtasocket->msocket->conn,mtasocket->port);
	if(ret<0)
	{
		printf("init socket failure,will return ...\n");
		return -1;
	}
	while(1)
	{
		memset(test.buff,0,128);

		mtasocket->msocket->read(mtasocket->msocket->conn, (char *)&test, sizeof(test));

		printf("get message from pid %d \n",test.pid );
		printf("get  message is %s\n", test.buff);

		test.pid = getpid();

		memset(test.buff,0,128);
		strcpy(test.buff,"This is socket test buff from main");

		sleep(2);

		mtasocket->msocket->write(mtasocket->msocket->conn, (char *)&test, sizeof(test));
	}

	middle_to_audio_socket_end(mtasocket);	
	return 0;

}
#endif

#include "message_parse.h"

#include<stdio.h>  
#include<stdlib.h>  
#include<pthread.h>  

void* server_function(void* v_message)  
{  
	struct test_message g_message ;
	int length = sizeof(g_message);
	struct message_socket *module = (struct message_socket *)v_message;
	struct message_socket to_module = module[0];
	int ret = -1;
	while(1)
	{
		ret = to_module.read(to_module.sockfd,&g_message,length);		
		printf("[MIDDLE]  get message from pid %d  is %s,ret=%d \n",g_message.pid ,g_message.buff,ret);
		sleep(5);

		ret = message_parse_send(&g_message,v_message,length);
		sleep(10);
	}
}

void* audio_function(void* v_message)  
{  
	struct test_message g_message ;
	int length = sizeof(g_message);
	struct message_socket *module = (struct message_socket *)v_message;
	struct message_socket to_module = module[1];
	int ret = -1;
	while(1)
	{
		ret = to_module.read(to_module.sockfd,&g_message,length);		
		printf("[MIDDLE]  get message from pid %d  is %s,ret=%d \n",g_message.pid ,g_message.buff,ret);
		sleep(5);

		ret = message_parse_send(&g_message,v_message,length);
		sleep(10);
	}
}

void* wifi_function(void* v_message)  
{  
	struct test_message g_message ;
	int length = sizeof(g_message);
	struct message_socket *module = (struct message_socket *)v_message;
	struct message_socket to_module = module[2];
	int ret = -1;
	while(1)
	{
		ret = to_module.read(to_module.sockfd,&g_message,length);		
		printf("[MIDDLE]  get message from pid %d  is %s,ret=%d \n",g_message.pid ,g_message.buff,ret);
		sleep(5);

		ret = message_parse_send(&g_message,v_message,length);
		sleep(10);
	}
}

void* bluetooth_function(void* v_message)  
{
	struct test_message g_message ;
	int length = sizeof(g_message);
	struct message_socket *module = (struct message_socket *)v_message;
	struct message_socket to_module = module[3];
	int ret = -1;
	while(1)
	{
		ret = to_module.read(to_module.sockfd,&g_message,length);		
		printf("[MIDDLE]  get message from pid %d  is %s,ret=%d \n",g_message.pid ,g_message.buff,ret);
		sleep(5);

		ret = message_parse_send(&g_message,v_message,length);
		sleep(10);
	}
}
  
int main(int argc,char **argv)  
{  
	struct test_message test_to_server;	
	int l_server = sizeof(struct test_message);
	test_to_server.length = l_server;
	test_to_server.m_type = MIDDLE_TO_SERVER;
	test_to_server.pid = getpid();
	
	struct test_message test_to_audio;	
	int l_audio = sizeof(struct test_message);
	test_to_audio.length = l_audio;
	test_to_audio.m_type = MIDDLE_TO_AUDIO;
	test_to_server.pid = getpid();
	
	struct test_message test_to_wifi;	
	int l_wifi = sizeof(struct test_message);
	test_to_wifi.length = l_wifi;
	test_to_wifi.m_type = MIDDLE_TO_WIFI;
	test_to_wifi.pid = getpid();
	
	struct test_message test_to_bluetooth;	
	int l_bluetooth = sizeof(struct test_message);
	test_to_bluetooth.length = l_bluetooth;
	test_to_bluetooth.m_type = MIDDLE_TO_BLUETOOTH;
	test_to_bluetooth.pid = getpid();

#if 0
	struct test_message test_from_server; 
	int f_server = sizeof(struct test_message);
	test_from_server.length = f_server;

	struct test_message test_from_audio; 
	int f_audio = sizeof(struct test_message);
	test_from_audio.length = f_audio;

	struct test_message test_from_wifi; 
	int f_wifi = sizeof(struct test_message);
	test_from_wifi.length = f_wifi;

	struct test_message test_from_bluetooth; 
	int f_bluetooth = sizeof(struct test_message);
	test_from_bluetooth.length = f_bluetooth;
#endif

	memset(test_to_server.buff,0,128);
//	strcpy(test_to_server.buff,"This is socket test buff from main to server");
	sprintf(test_to_server.buff,"[MIDDLE]  This is socket test buff from main to pid %d",test_to_server.pid);

	memset(test_to_audio.buff,0,128);
//	strcpy(test_to_audio.buff,"This is socket test buff from main to server");
	sprintf(test_to_audio.buff,"[MIDDLE]  This is socket test buff from main to pid %d",test_to_audio.pid);

	memset(test_to_wifi.buff,0,128);
//	strcpy(test_to_wifi.buff,"This is socket test buff from main to server");
	sprintf(test_to_wifi.buff,"[MIDDLE]  This is socket test buff from main to pid %d",test_to_wifi.pid);

	memset(test_to_bluetooth.buff,0,128);
//	strcpy(test_to_bluetooth.buff,"This is socket test buff from main to server");
	sprintf(test_to_bluetooth.buff,"[MIDDLE]  This is socket test buff from main to pid %d",test_to_bluetooth.pid);

	//为每个模块的read函数创建一个线程，如果采用异步机制一个线程就OK了
	pthread_t thread_server,thread_audio,thread_wifi,thread_bluetooth;  

	int pth1,pth2,pth3,pth4;  
	struct message_socket module[4] = {0};
	struct message_socket *server;
	struct message_socket *audio;
	struct message_socket *wifi;
	struct message_socket *blue;

	int ret = start_middle_to_module_service(server);
	if(ret == -1)
	{
		printf("[MIDDLE]  start server failure!\n");
		return -1;
	}
	ret = set_module_port(server, MIDDLE_TO_SERVER_PORT);
	if(ret == -1)
	{
		printf("[MIDDLE]  start server failure!\n");
		return -1;
	}
//	ret = middle_connect_module(server);
	int sfd = 0;
	int conn = 0;
//	ret = server->init(&(server->sockfd),&(server->conn),server->port);
	ret = server->init(&sfd,&conn,server->port);

	if(ret == -1)
	{
		printf("[MIDDLE]  start server failure!\n");
		return -1;
	}

	start_middle_to_module_service(audio);
	set_module_port(audio, MIDDLE_TO_AUDIO_PORT);
	middle_connect_module(audio);

	start_middle_to_module_service(wifi);
	set_module_port(wifi, MIDDLE_TO_WIFI_PORT);
	middle_connect_module(wifi);

	start_middle_to_module_service(blue);
	set_module_port(blue, MIDDLE_TO_BLUETOOTH_PORT);
	middle_connect_module(blue);

//	module[0] = server;
//	module[1] = audio;
//	module[2] = wifi;
//	module[3] = blue;
//	start_socket_server();
	memcpy(module,server,sizeof(server));
	memcpy(module+1,audio,sizeof(server));
	memcpy(module+2,wifi,sizeof(server));
	memcpy(module+3,blue,sizeof(server));


	pth1=pthread_create(&thread_server,NULL,server_function,module);  
	pth2=pthread_create(&thread_audio,NULL,audio_function,module);
	pth3=pthread_create(&thread_wifi,NULL,wifi_function,module);
	pth4=pthread_create(&thread_bluetooth,NULL,bluetooth_function,module);
	sleep(10);
	while(1)
	{
		int ret = message_parse_send(&test_to_server,&module,l_server);
		printf("[MIDDLE]  send:type:%d,pid:%d,length:%d,mess:%s ret = %d\n",test_to_server.m_type,test_to_server.pid,test_to_server.length,test_to_server.buff,ret);
		sleep(10);
		
		ret = message_parse_send(&test_to_audio,&module,l_audio);
		printf("[MIDDLE]  send:type:%d,pid:%d,length:%d,mess:%s ret = %d\n",test_to_audio.m_type,test_to_audio.pid,test_to_audio.length,test_to_audio.buff,ret);
		sleep(10);
		
		ret = message_parse_send(&test_to_wifi,&module,l_wifi);
		printf("[MIDDLE]  send:type:%d,pid:%d,length:%d,mess:%s ret = %d\n",test_to_wifi.m_type,test_to_wifi.pid,test_to_wifi.length,test_to_wifi.buff,ret);
		sleep(10);
		
		ret = message_parse_send(&test_to_bluetooth,&module,l_bluetooth);
		printf("[MIDDLE]  send:type:%d,pid:%d,length:%d,mess:%s ret = %d\n",test_to_bluetooth.m_type,test_to_bluetooth.pid,test_to_bluetooth.length,test_to_bluetooth.buff,ret);
		sleep(10);

	}
	
	pthread_join(thread_server,NULL);  
	pthread_join(thread_audio,NULL); 
//	pthread_join(thread_wifi,NULL); 
//	pthread_join(thread_bluetooth,NULL); 
	printf("-----------------------------\n");  
	printf("Thread 1 returns: %d\n",pth1);  
	printf("-----------------------------\n");  


	exit(0);  
  
}  
  
