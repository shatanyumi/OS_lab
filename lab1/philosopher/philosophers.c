#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<unistd.h>

#define N 5

sem_t chopstick[N];
pthread_mutex_t mutex;
int philosophers[N] = {0,1,2,3,4};
pthread_t philo[N];

void delay(){
	int i = rand()%N;
	sleep(i);
}

void *eat_think(void *arg){
	int num = *(int*)arg;
	int left = num;
	int right = (num + 1 ) % N;
	
	while(1){
		printf("philosopher %d is thinking...\n",num);
		delay();
		printf("philosopher %d want to eat\n",num);

		pthread_mutex_lock(&mutex);
		sem_wait(&chopstick[left]);
		printf("philosopher % d fetchs chopstick %d\n",left);
		sem_wait(&chopstick[right]);
		printf("philosopher %d fetchs chopstick %d\n",num,right);
		pthread_mutex_unlock(&mutex);

		printf("philosopher %d is eating with %d,%d chopsticks.\n",num,left,right);
		delay();

		sem_post(&chopstick[left]);
		printf("philosopher %d release chopstick %d\n",num,left);
		sem_post(&chopstick[right]);
		printf("philosopher %d release chopstick %d\n",num,right);

		printf("%d finish eating\n",num);
	}
	return;
}
int main(){
	srand(time(NULL));
	int i;	
	
	for(i = 0;i<N;i++){
		sem_init(&chopstick[i],0,1);
	}	
	pthread_mutex_init(&mutex,NULL);
	
	for( i = 0; i < N; i ++){
		pthread_create(&philo[i],NULL,eat_think,&philosophers[i]);
	}
	
	for( i = 0; i< N; i++){
		pthread_join(philo[i],NULL);
	}
	
	for( i = 0; i < N; i++){
		sem_destroy(&chopstick[i]);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
