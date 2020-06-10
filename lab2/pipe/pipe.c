#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int fd[2];
	pid_t pid1,pid2;
	char read_buf[50],write_buf[50];
	if(pipe(fd) == -1){
		perror("pipe");
	}
	
	pid1 = fork();
	if(pid1 < 0){
	perror("fork");
	}
	else if(pid1 == 0){
		close(fd[0]);
		lockf(fd[1],1,0);
		sprintf(write_buf,"Child 1 process[%d] is writing a message!",getpid());
		write(fd[1],write_buf,50);
		lockf(fd[1],0,0);	
	}
	else{
		pid2 = fork();
		if(pid2 <0){
			perror("fork");
		}
		else if(pid2 == 0){
			close(fd[0]);
			lockf(fd[1],1,0);
			sprintf(write_buf,"Child 2 process[%d] is writing a message!",getpid());
			write(fd[1],write_buf,50);
			lockf(fd[1],0,0);
		}else{

			close(fd[1]);
			waitpid(pid1,NULL,0);
			
			waitpid(pid2,NULL,0);
			read(fd[0],read_buf,50);
			printf("%s\n",read_buf);
			read(fd[0],read_buf,50);
			printf("%s\n",read_buf);
		
		}
	}

	return 0;
}
