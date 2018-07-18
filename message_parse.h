#ifndef __MESSAGE_PARSE_H_
#define __MESSAGE_PARSE_H_

//#include "message.h"

int start_socket_server();

int start_conncet_server();
int start_conncet_audio();
int start_conncet_wifi();
int start_conncet_bluetooth();





/* 解析消息是属于哪一类消息 */
int base_message_parse(const void *message);

/* 解析收到的消息,然后判断是需要发送给哪一个模块的 */
int message_parse(const void *message,int length);

/* 解析从各个模块收到的消息，然后判断是需要发送给哪一个模块的 */
int message_from_model_parse(const void *message,int length);

int message_from_server_parse(void *buff,int length);
int message_from_audio_parse(void *buff,int length);
int message_from_wifi_parse(void *buff,int length);
int message_from_bluetooth_parse(void *buff,int length);





#if 0
int end_socket_server();

int s_message(char *message);
int message_parse(const char*message,messgae_type type,int (*send_message)(char *message));
#endif
#endif
