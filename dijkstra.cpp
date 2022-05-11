#include "bots.h"
#include <iostream>

float cAStar::heuristicChoice(int choice, int x, int y)
{
	if (choice == 1) // Use the manhattan distance
	{
		return fabs(gTarget.PositionX() - x) + fabs(gTarget.PositionY() - y);
	}
	else if (choice == 2) // Use the diagonal distance
	{
		// Storing dx and dy to save calculating them twice
		float dx = fabs(gTarget.PositionX() - x);
		float dy = fabs(gTarget.PositionY() - y);
		return (dx + dy) - (0.6 * std::min(dx, dy));
	}
	else if (choice == 3) // Use the euclidean distance
	{
		return sqrt(pow(gTarget.PositionX() - x, 2) + pow(gTarget.PositionY() - y, 2));
	}
}

void cAStar::Build(cBotBase& bot, int heuristicNumber)
{
	// Go throigh the level and set the values
	for (int i = 0; i < GRIDWIDTH; i++)
	{
		for (int j = 0; j < GRIDHEIGHT; j++)
		{
			closed[i][j] = false; // No cells should be closed at first
			cost[i][j] = 100000.f; // Set the cost to a high value
			// LinkX/Y used to link the cell to the previous cell, but at first there are of course no links to default to -1
			linkX[i][j] = -1;
			linkY[i][j] = -1;
			inPath[i][j] = false; // No path exists at first (will be set to true later where there is one)
		}
	}

	// Set the cost of the bot's location to 0
	cost[bot.PositionX()][bot.PositionY()] = 0.f;
	
	bool keepLooping = true;
	while (keepLooping)
	{
		// Store the location of the lowest cost cell, used for closing the cell
		int lowestX;
		int lowestY;

		// Floats used for heuristics due to the possibility of floating point errors, 
		// especially with diagonal distance where decimal multiplication is used.
		float heuristic;
		float lowestHeuristic;
		
		// Reset lowest value each loop, else it will keep looping based on the previous lowest value,
		// which is zero on the first loop and hence will never find a new lowest value. 5000 has been chosen
		// as it is an insurmountable cost for a cell.
		float lowest = 5000.f;

		// Loop for each cell looking for the lowest cost
		for (int i = 0; i < GRIDWIDTH; i++) // For each row
		{
			for (int j = 0; j < GRIDHEIGHT; j++) // For each column
			{
				if (!closed[i][j]) // Ensure the cell is open
				{
					//heuristic = fabs(gTarget.PositionX() - i) + fabs(gTarget.PositionY() - j);
					heuristic = heuristicChoice(heuristicNumber, i, j);	
					if ((cost[i][j]) + heuristic < lowest) // If the cell is lower than the current lowest...
					{
						// Set the current lowest to the cell

						// Storing the cost...
						lowest = cost[i][j] + heuristic;
						lowestHeuristic = heuristic;

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
		
		// The heuristic has been removed post-closing due to the need for it to be maintained for comparison
		// to another cell's total with heuristic. Simple removing it at the end negates the need to add it onto
		// the cost the cell every time it is compared against another, instead turning it into single subtraction.
		
		lowest = lowest - lowestHeuristic; // Remove the lowest values' heuristic from the lowest value

		// Range from -1 to 1 for x and y, allowing all neighboring cells to be checked
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (gLevel.isValid(lowestX + i, lowestY + j)) // Check the cell exists - if on the edge of the level, it will not. Nor will it if it is a wall.
				{
					if (!closed[lowestX + i][lowestY + j]) // Don't check closed cells
					{
						// This section has been split as the cost for diagonal movement is different from straight movement.
						if (i != 0 && j != 0) // Diagonal (1.4 cost)
						{
							// If the new cost is lower than the current cost, set the new cost and link the parent cell to the current cell
							if (lowest + 1.4f < cost[lowestX + i][lowestY + j])
							{
								cost[lowestX + i][lowestY + j] = lowest + 1.4f;
								linkX[lowestX + i][lowestY + j] = lowestX;
								linkY[lowestX + i][lowestY + j] = lowestY;
							}
						}
						else // Horizontal or vertical (1 cost)
						{
							// If the new cost is lower than the current cost, set the new cost and link the parent cell to the current cell
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
		if (closed[gTarget.PositionX()][gTarget.PositionY()]) keepLooping = false; // If the target cell is closed, stop looping as we have found the path
	}

	bool done = false; //set to true when we are back at the bot position
	int nextClosedX = gTarget.PositionX(); //start of path
	int nextClosedY = gTarget.PositionY(); //start of path
	while (!done)
	{
		// This part traverses the path using the linkX and linkY arrays to find the next cell in the path
		inPath[nextClosedX][nextClosedY] = true;
		int tmpX = nextClosedX;
		int tmpY = nextClosedY;
		// Use the current location to find the next cell in the path using its linkX and linkY
		nextClosedX = linkX[tmpX][tmpY];
		nextClosedY = linkY[tmpX][tmpY];
		if ((nextClosedX == bot.PositionX()) && (nextClosedY == bot.PositionY())) done = true; // If we have reached the bot, stop looping as the traversal is complete
	}

	// Mark the path finding as complete
	completed = true;
}

cAStar gAStar;