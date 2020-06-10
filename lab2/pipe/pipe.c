#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int fd[2];
	pid_t pid;
	char read_buf[50],write_buf[50];
	if(pipe(fd) == -1){
		perror("pipe");
	}
	
	pid = fork();
	if(pid < 0){
	perror(fork);
	}
	else if(pid == 0){
		close(fd[0]);
		lockf(fd[1],1,0);
		sprintf(write_buf,"Child process is writing a message!");
		write(fd[1],write_buf,50);
		lockf(fd[1],0,0);	
	}
	else{
			close(fd[1]);
			waitpid(pid,NULL,0);
			read(fd[0],read_buf,50);
			printf("%s\n",read_buf);
	}

	return 0;
}
