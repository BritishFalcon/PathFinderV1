#include "bots.h"
#include <iostream>

void cDijkstra::Build(cBotBase &bot)
{
	// Go throigh the level and set the values
	for (int i = 0; i < GRIDWIDTH; i++)
	{
		for (int j = 0; j < GRIDHEIGHT; j++)
		{
			closed[i][j] = false;
			cost[i][j] = 10000.f;
			linkX[i][j] = -1;
			linkY[i][j] = -1;
			inPath[i][j] = false;
		}
	}
	
	// Set the cost of the bot's location to 0
	cost[bot.PositionX()][bot.PositionY()] = 0.f;
	bool keepLooping = true;
	float lowest = 1000000.f;
	float previousLowest;
	int lowestX;
	int lowestY;
	int checkPositionX;
	int checkPositionY;

	int test = 0;
	while (keepLooping)
	{
		// Reset lowest value each loop, else it will keep looping based on the previous lowest value,
		// which is zero on the first loop and hence will never find a new lowest value.
		lowest = 1000000.f;
		
		// Loop for each cell looking for the lowest cost
		for (int i = 0; i < GRIDWIDTH; i++) // For each row
		{
			for (int j = 0; j < GRIDHEIGHT; j++) // For each column
			{
				if (!closed[i][j]) // Ensure the cell is open
				{
					if (cost[i][j] < lowest) // If the cell is lower than the current lowest...
					{
						// Set the current lowest to the cell
						
						// Storing the cost...
						lowest = cost[i][j];
						
						// And the x and y coordinates
						lowestX = i;
						lowestY = j;

						// The co-ordinates of the cell and the cost have been stored seperately in order
						// to allow for the use of the co-ordinates elsewhere (in the next step).
					}
				}
			}
		}
		closed[lowestX][lowestY] = true; // Close the cell
		
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (gLevel.isValid(lowestX + i, lowestY + j))
				{
					if (!closed[lowestX + i][lowestY + j])
					{
						if (i != 0 && j != 0) // Diagonal
						{
							if (lowest + 1.4f < cost[lowestX + i][lowestY + j])
							{
								cost[lowestX + i][lowestY + j] = lowest + 1.4f;
								linkX[lowestX + i][lowestY + j] = lowestX;
								linkY[lowestX + i][lowestY + j] = lowestY;
							}
						}
						else // Horizontal or vertical
						{
							if (lowest + 1.0f < cost[lowestX + i][lowestY + j])
							{
								cost[lowestX + i][lowestY + j] = lowest + 1.0f;
								linkX[lowestX + i][lowestY + j] = lowestX;
								linkY[lowestX + i][lowestY + j] = lowestY;
							}
						}
					}
				}
			}
		}
		if (closed[gTarget.PositionX()][gTarget.PositionY()]) keepLooping = false;
	}

	bool done = false; //set to true when we are back at the bot position
	int nextClosedX = gTarget.PositionX(); //start of path
	int nextClosedY = gTarget.PositionY(); //start of path
	while (!done)
	{
		inPath[nextClosedX][nextClosedY] = true;
		int tmpX = nextClosedX;
		int tmpY = nextClosedY;
		nextClosedX = linkX[tmpX][tmpY];
		nextClosedY = linkY[tmpX][tmpY];
		if ((nextClosedX == bot.PositionX()) && (nextClosedY == bot.PositionY())) done = true;
	}
	
	completed = true;
}

cDijkstra gDijkstra;