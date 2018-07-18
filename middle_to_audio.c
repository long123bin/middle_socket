#include "middle_to_audio.h"

struct middle_to_audio_socket * middle_to_audio_socket_init()
{
	struct middle_to_audio_socket *mta = (struct middle_to_audio_socket *)malloc(sizeof(struct middle_to_audio_socket));
	if(!mta)
		return NULL;

	mta->msocket = message_socket_init();
	if(mta->msocket == NULL)
		return NULL;
	
//	mta->msocket->sockfd = -1;
//	mta->msocket->conn = -1;
//	mta->msocket->port = -1;

//	mta->port = MIDDLE_TO_AUDIO_PORT;
//	mta->msocket->port = mta->port;	

	return mta;

}

int middle_to_audio_socket_end(struct middle_to_audio_socket *message)
{
	if(!message || !message->msocket)
		
		return -1;
//	message->msocket->sockfd = -1;
//	message->msocket->conn = -1;
//	message->msocket->port = -1;

	message_socket_end(message->msocket);
	free(message);

	return 0;
}

