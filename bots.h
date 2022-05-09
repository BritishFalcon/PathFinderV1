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
	bool closed[GRIDWIDTH][GRIDHEIGHT]; //whether or not location is closed
	float cost[GRIDWIDTH][GRIDHEIGHT]; //cost value for each location
	int linkX[GRIDWIDTH][GRIDHEIGHT]; //link X coord for each location
	int linkY[GRIDWIDTH][GRIDHEIGHT]; //link Y coord for each location
	bool inPath[GRIDWIDTH][GRIDHEIGHT]; //whether or not a location is in the final path

	bool completed;

	virtual void Build(cBotBase &bot);
};

extern cDijkstra gDijkstra;