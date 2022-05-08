//======================================================================================
//Source code for a bot that randomly moves around, plus other bots as required
//
//(c) Patrick Dickinson, University of Lincoln, School of Computer Science, 2020
//======================================================================================

#include "bots.h"
#include "stdlib.h"
#include <iostream>

void cBotRandom::ChooseNextGridPosition() 
{
	//======================================================================================
	// We use gLevel to access environment layout
	// And gTarget to get target position
	// This function is called when a bot arrives at its current target location, to set the 
	// next grid position to reach the final target  
	//======================================================================================
	
	bool done = false;
	//Use while loop, as randomly selected location might not be valid
	while (!done)
	{
		int random = rand() % 4;
		if (random==0) done = SetNext((PositionX() + 1), (PositionY()), gLevel);
		else if (random == 1) done = SetNext((PositionX() - 1), (PositionY()), gLevel);
		else if (random == 2) done = SetNext((PositionX()), (PositionY() + 1), gLevel);
		else done = SetNext((PositionX()), (PositionY() - 1), gLevel);
	}
}

void cBotSimple::ChooseNextGridPosition()
{
	int moveX = 0, moveY = 0;
	bool stuckX = false, stuckY = false;
	if (gTarget.PositionX() != PositionX())
	{
		if (gTarget.PositionX() > PositionX())
		{
			if (gLevel.isValid(PositionX() + 1, PositionY())) moveX = 1; else stuckX = true;
		}
		else if (gLevel.isValid(PositionX() - 1, PositionY())) moveX = -1; else stuckX = true;
	}
	
	if (gTarget.PositionY() != PositionY())
	{
		if (gTarget.PositionY() > PositionY())
		{
			if (gLevel.isValid(PositionX(), PositionY() + 1)) moveY = 1; else stuckY = true;
		}
		else if (gLevel.isValid(PositionX(), PositionY() - 1)) moveY = -1; else stuckY = true;
	}
	
	SetNext(PositionX() + moveX, PositionY() + moveY, gLevel);
}

void cBotSimple2::ChooseNextGridPosition()
{
	int moveX = 0, moveY = 0;
	if (stuckX) moveY = directionY;
	if (stuckY) moveX = directionX;
	stuckX = false, stuckY = false;
	std::cout << "X: " << moveX << " Y: " << moveY << std::endl;
	
	if (moveX == 0)
	{
		if (gTarget.PositionX() != PositionX())
		{
			if (gTarget.PositionX() > PositionX())
			{
				if (gLevel.isValid(PositionX() + 1, PositionY()))
				{
					moveX = 1;
					stuckX = false;
				}
				else stuckX = true;
			}
			else if (gLevel.isValid(PositionX() - 1, PositionY()))
			{
				moveX = -1;
				stuckX = false;
			}
			else stuckX = true;
		}
	}
	else if (!gLevel.isValid(PositionX() + moveX, PositionY())) moveX = 0; stuckX = true;

	std::cout << moveY << std::endl;
	if (moveY == 0)
	{
		if (gTarget.PositionY() != PositionY())
		{
			if (gTarget.PositionY() > PositionY())
			{
				if (gLevel.isValid(PositionX(), PositionY() + 1))
				{
					moveY = 1;
					stuckY = false;
				}
				else stuckY = true;
			}
			else if (gLevel.isValid(PositionX(), PositionY() - 1))
			{
				moveY = -1;
				stuckX = false;
			}
			else stuckY = true;
		}
	}
	else if (!gLevel.isValid(PositionX(), PositionY() + moveY)) moveY = 0; stuckY = true;
	
	if (moveY != 0) directionX = moveX;
	if (moveX != 0) directionY = moveY;
	
	SetNext(PositionX() + moveX, PositionY() + moveY, gLevel);
}