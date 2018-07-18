#ifndef __MESSAGE_PARSE_H_
#define __MESSAGE_PARSE_H_

//#include "message.h"

#if 0
int start_socket_server();

/* 解析消息是属于哪一类消息 */
int base_message_parse(const void *message);

/* 解析收到的消息,然后判断是需要发送给哪一个模块的 */
int message_parse(const void *message,int length);

/* 解析从各个模块收到的消息，然后判断是需要发送给哪一个模块的 */
int message_from_module_parse(const void *message,int length);

int message_from_server_parse(void *buff,int length);
int message_from_audio_parse(void *buff,int length);
int message_from_wifi_parse(void *buff,int length);
int message_from_bluetooth_parse(void *buff,int length);
int end_socket_server();
#endif


#include "send_read_socket.h"

#define MIDDLE_TO_SERVER_PORT  8891
#define MIDDLE_TO_AUDIO_PORT  8887
#define MIDDLE_TO_WIFI_PORT  8888
#define MIDDLE_TO_BLUETOOTH_PORT  8889


int start_middle_to_module_service(struct message_socket *to_module);

int set_module_port(struct message_socket *to_module,int port);

int middle_connect_module(struct message_socket *to_module);
/* 解析消息是属于哪一类消息 */
int get_message_type(const void *message);

/* 解析收到的消息,然后判断是需要发送给哪一个模块的 */
int message_parse_send(const void *message,struct message_socket *to_module[],int length);


int end_middle_to_module_service(struct message_socket *end_module);



#if 0
int end_socket_server();

int s_message(char *message);
int message_parse(const char*message,messgae_type type,int (*send_message)(char *message));
#endif
#endif
