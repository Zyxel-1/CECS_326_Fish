//---------------------------------------------------------------------------------
//	Name: Roberto Sanchez
//	Id: 014587792
//	Date:Oct 12, 2017
//	Assignment: Lab-03-Shared_Memory
//	File: Swim_mill.c
//---------------------------------------------------------------------------------
#include "memoryPool.h"
//---------------------------------------------------------------------------------
#include <sys/types.h>	/* */
#include <sys/ipc.h>	/* */
#include <sys/shm.h>	/* */
#include <stdio.h>		/* Needed for input/output functions*/
#include <sys/wait.h>   /* Needed for the wait function */
#include <unistd.h>     /* Needed for the fork function */
#include <string.h>     /* Needed for the strcat function */
#include <stdbool.h>	/* Needed for booleans*/
#include <signal.h>		/* Needed for handling signals while executing programs*/
#include <stdlib.h>		/* Needed for standard library*/
#include <time.h> 		/* Needed for timer */
// Functions Definitions
void deAllocateMemory();
void printSwimLane(int);
void initializeSwimlane();
void CTR_C();
void timesUp();
// Declaring Variables
#define SHMSIZE 27 	
pid_t idPellet;
pid_t idFish;
pid_t ProcessPrint;
key_t key;
//---------------------------------------------------------------------------------
int main(int argc, char * argv[])
 {
 	int sharedMemoryID;
 	sharedMemoryID = shmget(MemKey, sizeof(char[10][10]), IPC_CREAT | 0666);
 	// Gets Memory
	sharedMemory();
	attachMemory();
	// Sets up signals
	signal(SIGINT,CTR_C);
	signal(SIGTERM,timesUp);
	// Calls alarm after 30 seconds
	alarm(30);
	// Sets up timer
	int timer = 0;
	int timeLimit = 30;
	// Starts Program
	initializeSwimlane();
	// Forks idFish 
	pid_t idFish = fork();

	if(idFish == 0)
		execv("fish",argv);
	else
	{
		sleep(1);
		pid_t ProcessPrint = fork();
		if(ProcessPrint == 0)
		{
			while(timer < timeLimit)
			{
				printSwimLane(timer);
				wait(NULL);
				timer++;
				sleep(1);
			}
		}

		while(timer < timeLimit)
		{
			pid_t idPellet = fork();
			if(idPellet == 0)
			{
				execv("pellet",argv);
			}
		srand(time(NULL));
		sleep(3);
		}
	}
	// Deallocates Memory
	deAllocateMemory();
	return 0;
}
//---------------------------------------------------------------------------------
// Functions includes:
//		- void deAllocateMemory();
//		- void printSwimLane();
//		- void initializeSwimlane();
//		- void CTR_C();
//		- void timesUp();
//---------------------------------------------------------------------------------
//Purpose:
//Outputs:
//Inputs:
void deAllocateMemory(){
	shmdt(Swimlane);
	shmctl(sharedMemoryID,IPC_RMID,0);
	kill(0,SIGTERM);
}
//---------------------------------------------------------------------------------
//Purpose:
//Outputs:
//Inputs:
void printSwimLane(int timer){
printf("\nTimer:%i\n",timer);
				for(int i = 0; i< 10; i++){
					for(int j = 0; j < 10; j++){
						printf("%c ", * Swimlane[i][j]);
					}
					printf("\n");
				}
}
//---------------------------------------------------------------------------------
//Purpose: Creates the characters in the swimlane matrix. It sets it to '~' character
//Outputs:
//Inputs:
void initializeSwimlane(){
	printf("\nCreating SwimLane:\n");
	for(int i = 0; i< 10; i++){
		for(int j = 0; j < 10; j++){
			* Swimlane[i][j] = '~';
		}
	}
}
//---------------------------------------------------------------------------------
//Purpose: Handles interruptions when User enters CTRL+C
//Outputs: Prompts User that it has ended
//Inputs: None
void CTR_C(){
	printf("Program ENDED by user (CTR+C was detected).\n Pellet ID: %d \n",getpid());
	shmdt(Swimlane);
	shmctl(sharedMemoryID, IPC_RMID,NULL);
	while(wait(NULL)>0)
	{
		printf("Pellet Processed\n");
	}
	exit(0);
}
//---------------------------------------------------------------------------------
//Purpose: Handles when the timer hits its limit.
//Outputs: Prompts user that the program as ended
//Inputs: None
void timesUp(){
	printf("\nTimes UP\nPellet ID: %d\n",getpid());
	shmdt(Swimlane);
	shmctl(sharedMemoryID,IPC_RMID,NULL);
	while(wait(NULL)>0){
		printf("Pellet Processed\n");
	}
	exit(0);
}