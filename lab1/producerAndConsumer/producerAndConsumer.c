#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define PRODUCER_NUM 3
#define CONSUMER_NUM 4
#define BUF_SIZE 10
#define SOURCE_FILE "sources.txt"

FILE *fp;
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int producer_pthread = 0;
int consumer_pthread = 0;

char *buf[BUF_SIZE];

void *produce(void *arg){
	int n = *(int*)arg;
	while(1){
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		
		char source[40];
		if(feof(fp)){
			fseek(fp,0,SEEK_SET);
		}
		fscanf(fp,"%s\n",source);
		int pos = producer_pthread % BUF_SIZE;
		buf[pos] = source;
		producer_pthread ++;
		
		sem_post(&full);
		printf("producer %d produce %s in position %d\n",n,source,pos);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
}
void *consume(void *arg){
	int n = *(int*)arg;
	while(1){
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		
		int pos = consumer_pthread % BUF_SIZE;
		char *source = buf[pos];
		consumer_pthread++;

		sem_post(&empty);
		printf("consumer %d consume product %s in position %d\n",n,source,pos);
		pthread_mutex_unlock(&mutex);
		sleep(rand()%5);
	}
}
int main(){
	srand((unsigned)time(NULL));
	fp = fopen(SOURCE_FILE,"r");
	if(fp == NULL){
		printf("open file error\n");
		exit(1);
	}
	
	if(pthread_mutex_init(&mutex,NULL)==-1){
		printf("pthread_mutex_init mutex error\n");
		exit(1);
	}

	if(sem_init(&empty,0,BUF_SIZE) == -1){
		printf("sem_init empty error\n");
		exit(1);
	}
	
	if(sem_init(&full,0,0) == -1){
		printf("sem_init full error\n");
		exit(1);
	}
	
	pthread_t producer[PRODUCER_NUM];
	pthread_t consumer[CONSUMER_NUM];
	
	int i;
	for(i = 0; i < CONSUMER_NUM; i++){
		if(pthread_create(&consumer[i],0,consume,&i)==-1){
			printf("pthread_create consumer %d error\n",i);
			exit(1);
		}
	}
	
	for(i = 0; i< PRODUCER_NUM;i++){
		if(pthread_create(&producer[i],0,produce,&i)==-1){
			printf("pthread_create producer %d error\n",i);
			exit(1);
		}
	}
	
	for(i = 0; i<PRODUCER_NUM;i++){
		pthread_join(producer[i],NULL);
	}
	for(i = 0; i<CONSUMER_NUM;i++){
		pthread_join(consumer[i],NULL);
	}
	return 0;
}
