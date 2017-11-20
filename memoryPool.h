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
//---------------------------------------------------------------------------
// Variable Declaration
int sharedMemoryID;		  // Stores the actual array
key_t MemKey = 1234;      // keeps track of shared memory
char (*Swimlane)[10][10]; // Actual 2D array
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