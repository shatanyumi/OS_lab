#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/msg.h>

#define MAX_SIZE 1024
#define read_key 1234
#define send_key 1235

struct msg_st{
	long msg_type;
	char text[MAX_SIZE];
};

int main(){
	
	int msg_id_read = -1,msg_id_send = -1;
	struct msg_st msg_read,msg_send;
	long msgtype = 0;
	
	msg_id_send = msgget((key_t)send_key,0666 | IPC_CREAT);
	msg_id_read = msgget((key_t)read_key,0666 | IPC_CREAT);

	if(msg_id_send == -1|| msg_id_read == -1){
		printf("msgget failed with error:%d\n",errno);
		exit(1);
	}
	
	msg_send.msg_type = 1;
	sprintf(msg_send.text,"client's process id is %d",getpid());
	if(msgsnd(msg_id_send,(void*)&msg_send,MAX_SIZE,0) == -1)
	{
		printf("msgsnd failed\n");
		exit(1);
	}
	
	if(msgrcv(msg_id_read,(void*)&msg_read,BUFSIZ,msgtype,0)<0)
	{
		printf("msgrcv failed\n");
		exit(1);
	}
	printf("%s\n",msg_read.text);
	
	return 0;
}
