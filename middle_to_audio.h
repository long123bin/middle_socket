#ifndef __MIDDLE_TO_AUDIO_H_
#define __MIDDLE_TO_AUDIO_H_

#include "send_read_socket.h"

//#define MIDDLE_TO_AUDIO_PORT  8887

struct middle_to_audio_socket
{
	struct message_socket *msocket;
//	int port;
};

struct middle_to_audio_socket * middle_to_audio_socket_init();

int middle_to_audio_socket_end(struct middle_to_audio_socket *message);

#endif
