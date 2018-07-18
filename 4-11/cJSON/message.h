#ifndef __MESSAGE_H_
#define __MESSAGE_H_

#include <sys/types.h>
#include <unistd.h>

typedef enum Messgae_type 
{
	SERVER = 9990,
	AUDIO,
	WIFI,
	BLUETOOTH,
	
	MIDDLE_TO_SERVER = 10000,
	MIDDLE_TO_AUDIO,
	MIDDLE_TO_WIFI,
	MIDDLE_TO_BLUETOOTH,
	
	AUDIO_TO_SERVER = 10050,
	WIFI_TO_SERVER,
	BLUETOOTH_TO_SERVER,

	BLUETOOTH_TO_AUDIO = 10100,
	WIFI_TO_AUDIO,
	SERVER_TO_AUDIO,
	
	AUDIO_TO_WIFI,
	BLUETOOTH_TO_WIFI,
	SERVER_TO_WIFI,
	
	AUDIO_TO_BLUETOOTH,	
	WIFI_TO_BLUETOOTH,
	SERVER_TO_BLUETOOTH,

	AUDIO_TO_MIDDLE,	
	WIFI_TO_MIDDLE = 10110,
	SERVER_TO_MIDDLE,
	BLUETOOTH_TO_MIDDLE,

	

	TO_SERVER = 10200,
	TO_AUDIO,
	TO_WIFI,
	TO_BLUETOOTH,
	TO_MIDDLE,

	INVALID_TYPE = -1
}messgae_type;

struct test_message
{
//	char *test_data;
	enum Messgae_type  m_type;
	pid_t pid;
	char buff[128];
	int length;
	int port;
};

#endif
