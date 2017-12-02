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
void pelletMessage(bool, int, int);
//---------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	sharedMemory();
	attachMemory();
	attachSemaphore();

	bool eaten = false;
	srand(time(NULL));
	// Generating Random Values for (x,y) cordinate
	int i = rand() % 10;
	int j = rand() % 10;
	// I think if its either a 'F' or 'o' generate another values
	while(*Swimlane[i][j] != '~')
	{
		i = rand() % 10;
		j = rand() % 10;
	}
	lock();
	// Places pellet 'o' in x,y cordinate
	*Swimlane[i][j] = 'o';
	unlock();
	// Pelllet Movement as it falls "Down"
	while(i < 9)
	{
		// replace current pellet with '-'
		lock();
		*Swimlane[i][j] = '~';
		unlock();
		// Move down pellet
		++i;
		// If the pellet is on top of 'F' then its eaten
		// else replace slot with pellet
		lock();
		if(*Swimlane[i][j] == 'F')
		{
			eaten = true;
			break;
		}
		else
			*Swimlane[i][j] = 'o';
		unlock();
		sleep(1);
	}
	//I could move this into the while loop
	lock();
	if(eaten)
		*Swimlane[i][j] = 'F';
	else
		*Swimlane[i][j] = '~';
	unlock();
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