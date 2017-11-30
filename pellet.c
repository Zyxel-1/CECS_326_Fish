//---------------------------------------------------------------------------------
//	Name: Roberto Sanchez
//	Id: 014587792
//	Date:Oct 25, 2017
//	Assignment: Lab-03-Shared_Memory
//	File: pellet.c
//https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_73/apiref/apiexusmem.htm
//---------------------------------------------------------------------------------
#include "memoryPool.h"
#include <sys/sem.h>
int semid;
key_t semkey;
int NUMSEMS;
struct sembuf{
	int sem_num;
	short sem_op;
	short sem_flag;
};
struct sembuf WAIT[1];
struct sembuf SIGNAL[1];
WAIT[0].sem_num = 0;
WAIT[0].sem_op = -1;
WAIT[0].sem_flag = SEM_UNDO;
SIGNAL[0].sem_op = 1;
SIGNAL[0].sem_num = 0;
SIGNAL[0].sem_flag = SEM_UNDO;
void pelletMessage(bool, int, int);
//---------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	semkey = ftok("/dev/null",1);
	semid = semget( semkey, NUMSEMS, 0666 | IPC_CREAT | IPC_EXCL );
    if ( semid == -1 )
    {
        printf("main: semget() failed\n");
        return -1;
    }
	sharedMemory();
	attachMemory();
	bool eaten = false;
	srand(time(NULL));
	// Generating Random Values for (x,y) cordinate
	int i = rand() % 10;
	int j = rand() % 10;
	// I think if its either a 'F' or 'o' generate another values
	semop(semid,WAIT,1);
	while(*Swimlane[i][j] != '~')
	{
		i = rand() % 10;
		j = rand() % 10;
	}
	semop(semid,SIGNAL,1);
	// Places pellet 'o' in x,y cordinate
	semop(semid,WAIT,1);
	*Swimlane[i][j] = 'o';
	semop(semid,SIGNAL,1);
	// Pelllet Movement as it falls "Down"
	while(i < 9)
	{
		// replace current pellet with '-'
		semop(semid,WAIT,1);
		*Swimlane[i][j] = '~';
		semop(semid,SIGNAL,1);
		// Move down pellet
		++i;
		// If the pellet is on top of 'F' then its eaten
		// else replace slot with pellet
		semop(semid,WAIT,1);
		if(*Swimlane[i][j] == 'F')
		{
			eaten = true;
			break;
		}
		else
			*Swimlane[i][j] = 'o';
		semop(semid,SIGNAL,1);
		sleep(1);
	}
	//I could move this into the while loop
	semop(semid,WAIT,1);
	if(eaten)
		*Swimlane[i][j] = 'F';
	else
		*Swimlane[i][j] = '~';
	semop(semid,SIGNAL,1);
	//Outputs Message
	pelletMessage(eaten,i,j);	

}
//--------------------------------------------------------------------------------
//Purpose: Displays message when pellet is eaten or not
//Input: boolean if it was eaten and it's coordinates
//Output: Message whether its been eaten or not
void pelletMessage(bool eaten, int x, int y){
	if(eaten)
		printf("\nPellet: %d was eaten at ((%d),(%d) cordinate.\n",getpid(),x+1, y+1);
	else
		printf("\nPellet: %d was not eaten at ((%d),(%d) cordinate.\n",getpid(),x+1, y+1);
}