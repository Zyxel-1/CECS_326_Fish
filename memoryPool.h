//---------------------------------------------------------------------------------
//	Name: Roberto Sanchez
//	Id: 014587792
//	Date:Oct 12, 2017
//	Assignment: Lab-03-Shared_Memory
//	File: memoryPool.h
//---------------------------------------------------------------------------------
//Header file containing repeated methods for Fish, Pellet and Swimmill class
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <time.h>
#include <string.h>     /* Needed for the strcat function */
#include <semaphore.h>
#include <sys/sem.h>

//---------------------------------------------------------------------------
// Variable Declaration
int sharedMemoryID;		  // Stores the actual array
key_t MemKey = 1234;      // keeps track of shared memory
char (*Swimlane)[10][10]; // Actual 2D array
int semid;
int semnum = 1;
key_t semkey = 6161;
//---------------------------------------------------------------------------
void sharedMemory(){
	if ((sharedMemoryID = shmget(MemKey, sizeof(char[10][10]), 0666)) < 0)
	{ 
		perror("\nerror\n");
		exit(1);
	}
}
//---------------------------------------------------------------------------
void attachMemory()
{
	if ((Swimlane = (char (*)[10][10])shmat(sharedMemoryID, NULL, 0)) == (char (*)[10][10]) -1) 
	{
		perror("\n error \n");
	 	exit(1);
	}
}
//---------------------------------------------------------------------------
int getvalue()
{
    if ( semctl(semid,0,GETVAL) == -1)
    {
        //perror("\n error \n");
        exit (-1);
    }
    else
        return semctl(semid,0, GETVAL);
}
//---------------------------------------------------------------------------
int lock()
{
    struct sembuf sem_lock = { 0, -1, IPC_NOWAIT};
    if (semop(semid, &sem_lock, 1) < 0)
    {
        //perror("\n error \n");
        return 0;
    }
    else
        return 1;
}
//---------------------------------------------------------------------------
int unlock()
{
    struct sembuf sem_unlock = { 0, 1, IPC_NOWAIT};   
    if ( semop(semid, &sem_unlock, 1) < 0)
    {
        //perror("\n error \n");
        return 0;
    }
    else
        return 1;
}
//---------------------------------------------------------------------------
void setSemaphore()
{
    semid = semget(semkey, 1 , IPC_CREAT | 0666);
    if(semid < -1)
    {
        //perror("\n error \n");
        exit(1);
    }
}
//---------------------------------------------------------------------------
void attachSemaphore()
{
    semid = semget(semkey, 1, 0666);
    if(semid < 0)
    {
        //printf("\n error \n");
        exit(1);
    }
}
