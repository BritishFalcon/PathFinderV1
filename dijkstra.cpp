#include "bots.h"
#include <iostream>

cDijkstra::cDijkstra()
{ 
	completed = false; 
}

void cDijkstra::Build(cBotBase &bot)
{
	// Go throigh the level and set the values
	for (int i = 0; i < GRIDWIDTH; i++)
	{
		for (int j = 0; j < GRIDHEIGHT; j++)
		{
			closed[i][j] = false;
			cost[i][j] = 1000000.f;
			linkX[i][j] = -1;
			linkY[i][j] = -1;
			inPath[i][j] = false;
		}
	}
	
	bool keepLooping = false;
	float &lowest = cost[bot.PositionX()][bot.PositionY()];
	float& previousLowest = lowest;
	int lowestX = bot.PositionX();
	int lowestY = bot.PositionY();
	int checkPositionX = bot.PositionX();
	int checkPositionY = bot.PositionY();
	// Create a vector of two numbers both being 0

	
	while (keepLooping)
	{
		// Loop for each cell looking for the lowest cost
		for (int i = 0; i < GRIDWIDTH; i++)
		{
			for (int j = 0; j < GRIDHEIGHT; j++)
			{
				if (!closed[i][j])
				{
					if (cost[i][j] < lowest) lowest = cost[i][j], lowestX = i, lowestY = j;
				}
			}
		}
		closed[lowestX][lowestY] = true;
		
		// Step 3
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (gLevel.isValid(lowestX + i, lowestY + j))
				{
					if (!closed[i][j])
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
		if ((nextClosedX == bot.PositionX()) && (nextClosedY ==
			bot.PositionY())) done = true;
	}
	
	completed = true;
}

cDijkstra gDijkstra;