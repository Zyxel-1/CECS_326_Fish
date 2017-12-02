//---------------------------------------------------------------------------------
//	Name: Roberto Sanchez
//	Id: 014587792
//	Date:Oct 25, 2017
//	Assignment: Lab-03-Shared_Memory
//	File: fish.c
//---------------------------------------------------------------------------------
#include "memoryPool.h"
#include <sys/sem.h>
//---------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
	sharedMemory();
	attachMemory();
	attachSemaphore();
	// Placing fish into the middle of grid
	lock();
	* Swimlane[9][5] = 'F';
	unlock();
	// Running through the locations and movement of fish

	while(true)
	{
		// Finding fish position on the grid
     	int fishColumnPosition;
      	for (int i = 1; i < 10; i++) 
      	{
        	for (int j = 1; j < 10; j++) 
        	{
						lock();
          		if ( * Swimlane[i][j] == 'F') 
            		fishColumnPosition = j;
						unlock();
        	}
      	}
      	//Finding pellet row position on the grid
      	int pelletRowPosition = 10;
      	for (int i = 1; i < 10; i++) 
      	{
        	for (int j = 1; j < 10; j++) 
        	{
						lock();
        	  	if ( * Swimlane[i][j] == 'o')
        	  	{
           	 		if (abs(pelletRowPosition - 9) < abs(i - 9))
          	   			pelletRowPosition = i;
          		}
						unlock();
        	}
      	}
	    //Finds the closest pellet in the column
	    int closePelletCol = 10;
	    for (int i = 1; i < 10; i++) 
	    {
	        for (int j = 1; j < 10; j++) 
	        {
						lock();
	          if ( * Swimlane[i][j] == 'o') 
	          {
	            if (abs(closePelletCol - fishColumnPosition) > abs(j - fishColumnPosition)) 
	              closePelletCol = j;
	          }
						unlock();
	        }
	    }
//-------------------------------------------------------------------------------------------
//	Figures out the track to pellet
//-------------------------------------------------------------------------------------------
      	if (closePelletCol != 10)
      	{
        	int moveFish = closePelletCol - fishColumnPosition;
        	while (abs(moveFish))
        	{
          		int pelletRowPosition = 10;
          		for (int i = 1; i < 10; i++)
          		{
            		for (int j = 1; j < 10; j++)
            		{
									lock();
              			if ( * Swimlane[i][j] == 'o')
              			{
                			if (abs(pelletRowPosition - 9) < abs(i - 9))
                  				break;
              			}
									unlock();
            		}
          		}
          		// replace fish with ~
							lock();
          		* Swimlane[9][fishColumnPosition] = '~';
							unlock();
          		//Helps determine where to move the fish
          		if (moveFish <= 0)
            		fishColumnPosition--;
          		else
            		fishColumnPosition++;
            	// Place Fish once moved
							lock();
				* Swimlane[9][fishColumnPosition] = 'F';
				unlock();
          		if (moveFish <= 0)
            		moveFish++;
          		else 
            		moveFish--;
          		sleep(1);
	        }
        }else
      	{
        	int moveFish = fishColumnPosition - 5;
	        while (abs(moveFish))
	        {
	          int pelletRowPosition = 10;
	          for (int i = 0; i < 10; i++)
	          {
	            for (int j = 0; j < 10; j++)
	            {
								lock();
	              if ( * Swimlane[i][j] == 'o')
	              {
	                if (abs(pelletRowPosition - 9) < abs(i - 9))
	                  break;
	              }
								unlock();
	            }
	          }
						lock();
	          * Swimlane[9][fishColumnPosition] = '~'; 
						unlock();
	          if (moveFish <= 0)
	            fishColumnPosition++;
	          else if (moveFish >= 0)
	            fishColumnPosition--;
						lock();
	          * Swimlane[9][fishColumnPosition] = 'F';
						unlock();
	          if (moveFish <= 0)
	            moveFish++;
	          else if (moveFish >= 0)
	            moveFish--;
	          sleep(1);
        	}
      	}
     sleep(1);
	}
	return 0;
}