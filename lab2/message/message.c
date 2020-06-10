#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#define MAX_TEXT 1024

struct msg_st
{
	long msg_type;
	char text[MAX_TEXT];
};

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
		msgid = msgget((key_t)1234,0666|IPC_CREAT);
		if(msgid<0){
			fprintf(stderr,"msgget failed with errir %d\n",errno);
			exit(EXIT_FAILURE);
		}
		while(running){
			printf("Enter some text:\n");
			fgets(buffer,BUFSIZ,stdin);
			data.msg_type = 1;
			sprintf(data.text,"Child process %d write message: %s",getpid(),buffer);
			if(msgsnd(msgid,(void*)&data,MAX_TEXT,0) < 0){
				fprintf(stderr,"msgnd failed\n");
				exit(EXIT_FAILURE);
			}
			if(strncmp(buffer,"end",3)==0){
				running = 0;
			}
			sleep(1);
		}
		exit(EXIT_SUCCESS);
		
	}
	else{
		waitpid(pid,NULL,0);	
		msgid = msgget((key_t)1234,0666 | IPC_CREAT);
		if(msgid < 0){
			fprintf(stderr,"msgget failed with error: %d\n",errno);
			exit(EXIT_FAILURE);
		}
		
		while(running){
			if(msgrcv(msgid,(void*)&data,BUFSIZ,msgtype,0)<0){
				fprintf(stderr,"msgrcv failed with errno: %d\n",errno);
				exit(EXIT_FAILURE);
			}
			printf("Parent process recieve: \nYou wrote:%s\n",data.text);
			if(strncmp(data.text,"end",3) == 0){
				running = 0;
			}
		}
		if(msgctl(msgid,IPC_RMID,0) < 0){
			fprintf(stderr,"msgctl(IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);	
	}
	return 0;
}
