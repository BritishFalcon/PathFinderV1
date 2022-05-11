#pragma once
//======================================================================================
//Header file for bots: initially cBotRandom is defined here, but other bot classes
//can go here too
//
//(c) Patrick Dickinson, University of Lincoln, School of Computer Science, 2020
//======================================================================================

#include "botbase.h"

class cBotRandom : public cBotBase
{
	virtual void ChooseNextGridPosition();
};

class cDijkstra
{
public:
	// Initialise the variables in accordance to the grid size
	bool closed[GRIDWIDTH][GRIDHEIGHT]; //whether or not location is closed
	float cost[GRIDWIDTH][GRIDHEIGHT]; //cost value for each location
	int linkX[GRIDWIDTH][GRIDHEIGHT]; //link X coord for each location
	int linkY[GRIDWIDTH][GRIDHEIGHT]; //link Y coord for each location
	bool inPath[GRIDWIDTH][GRIDHEIGHT]; //whether or not a location is in the final path

	bool completed;

	// Set completed to false on creation
	inline cDijkstra() { completed = false; }
};

class cAStar : public cDijkstra // Inherit the variables from cDijkstra as the implementation is very similar
{
public:
	virtual void Build(cBotBase &bot, int heuristicChoice);
	
	// The heuristic function has no real use outside of this class
protected:
	float heuristicChoice(int choice, int i, int j);
};

extern cAStar gAStar;

// Dijkstra declaration no longer required as it has been replaced by A*