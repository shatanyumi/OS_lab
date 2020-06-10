#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#define MAX_TEXT 1024

int exit_sig = 0;

struct msg_st
{
	long msg_type;
	char text[MAX_TEXT];
};

void server(pid_t pid){
	struct msg_st data;
	int msgid = -1;
	long msgtype = 0;
	
	msgid = msgget((key_t)1234,0666 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr,"msgid get errir\n");
		return;
	}

	if(msgrcv(msgid,(void*)&data,BUFSIZ,msgtype,0)<0){
		fprintf(stderr,"msgrcv failed\n");
		exit(1);
	}
	printf("%s\n",data.text);
	
	data.msg_type = 1;
	sprintf(data.text,"server's process pid is %d",pid);
	if(msgsnd(msgid,(void*)&data,MAX_TEXT,0)<0){
		fprintf(stderr,"msgsnd failed\n");
		exit(1);
	}

	return;
	
}

void client(pid_t pid){
	struct msg_st data;
	int msgid = -1;
	long msgtype = 0;
	
	msgid = msgget((key_t)1234,0666|IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr,"msgid get error\n");
		return;
	}
	data.msg_type = 1;
	sprintf(data.text,"client's process pid is %d",pid);
	if(msgsnd(msgid,(void *)&data,MAX_TEXT,0)<0){
		fprintf(stderr,"msgsnd failed\n");
		exit(1);
	}
	
	if(msgrcv(msgid,(void*)&data,BUFSIZ,msgtype,0)<0){
		fprintf(stderr,"msgrcv failed\n");
		exit(1);
	}
	printf("%s\n",data.text);	
	exit_sig = 1;
	return;
}
int main(){
	int running = 1;
	struct msg_st data;
	char buffer[BUFSIZ];	
	int msgid = -1;
	long msgtype = 0;

	pid_t pid = fork();
	if(pid < -1){
	
	}
	else if(pid == 0){
		printf("Child process as server:\n");
		while(!exit_sig){
			server(getpid());
		}
		exit(0);
	}
	else{
		printf("Parent process as client:\n");
		client(getpid());
		exit(0);
	}
	return 0;
}
