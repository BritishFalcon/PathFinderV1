//======================================================================================
//Source code for main function = main control loop for programme
//
//(c) Patrick Dickinson, University of Lincoln, School of Computer Science, 2020
//
//Build from original templace by Ben Griffiths (25199556) for Artificial Intelligence Assessment 2
//https://github.com/BritishFalcon/PathFinderV1 (private until due date) 
//======================================================================================

#include "game.h"
#include "level.h"
#include "dynamic.h"
#include "bots.h"
#include <iostream>
#include <limits>
//======================================================================================
//Globals
//======================================================================================
SDL_Window *gMainWindow = NULL;
SDL_Renderer *gMainRenderer = NULL;
SDL_Surface *tileSurface = NULL;
SDL_Surface* tileBlockedSurface = NULL;
SDL_Surface* targetSurface = NULL;
SDL_Surface* botSurface = NULL;
SDL_Surface* tileClosedSurface = NULL;
SDL_Surface* tileRouteSurface = NULL;
SDL_Texture* tileTexture = NULL;
SDL_Texture* tileBlockedTexture = NULL;
SDL_Texture* targetTexture = NULL;
SDL_Texture* botTexture = NULL;
SDL_Texture* tileClosedTexture = NULL;
SDL_Texture* tileRouteTexture = NULL;
bool gQuit;


//======================================================================================
//Main function
//======================================================================================
int main(int argc, char* argv[])
{
    gQuit = false;

    //======================================================================================
    //Select a heuristic
    //======================================================================================

    int heuristic;
    bool valid = false;
    while (!valid)
    {
        // Ask user to select a heuristic, and check that it is valid
        std::cout << "Please choose from one of the following heuristics:" << std::endl;
        std::cout << "1. Manhattan distance" << std::endl;
        std::cout << "2. Diagonal distance" << std::endl;
        std::cout << "3. Euclidean distance" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> heuristic;
		
        // Clear the input buffer to prevent infinite looping in the event of an error
        std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Verify input is valid
        if (heuristic == 1 || heuristic == 2 || heuristic == 3) valid = true;
		// If not, print error message and try again
        else heuristic = 0, std::cout << "Invalid choice, please retry.\n" << std::endl;
    }
	
    //======================================================================================
    //Initialise SDL
    //======================================================================================
    SDL_Init(SDL_INIT_EVERYTHING);
    gMainWindow = SDL_CreateWindow
    ("Pathfinder", // window's title
        30, 50, // coordinates on the screen, in pixels, of the window's upper left corner
        640, 640, // window's length and height in pixels  
        SDL_WINDOW_OPENGL);
    gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED);

    //======================================================================================
    //Load graphics for tiles, bot and target
    //======================================================================================
    tileSurface = SDL_LoadBMP("tile.bmp");
    tileBlockedSurface = SDL_LoadBMP("tile-blocked.bmp");
    targetSurface = SDL_LoadBMP("target.bmp");
    botSurface = SDL_LoadBMP("bot.bmp");
    tileClosedSurface = SDL_LoadBMP("tile-closed.bmp");
    tileRouteSurface = SDL_LoadBMP("tile-route.bmp");

    tileTexture = SDL_CreateTextureFromSurface(gMainRenderer, tileSurface);
    tileBlockedTexture = SDL_CreateTextureFromSurface(gMainRenderer, tileBlockedSurface);
    targetTexture = SDL_CreateTextureFromSurface(gMainRenderer, targetSurface);
    botTexture = SDL_CreateTextureFromSurface(gMainRenderer, botSurface);
    tileClosedTexture = SDL_CreateTextureFromSurface(gMainRenderer, tileClosedSurface);
    tileRouteTexture = SDL_CreateTextureFromSurface(gMainRenderer, tileRouteSurface);

    //======================================================================================
    //Load the map and set target position
    //======================================================================================
    gLevel.Load("maps/1.txt");
    gTarget.SetCurrent(30, 20, gLevel);

    //======================================================================================
    //Locals variables fro key presses and frame timer
    //======================================================================================
    SDL_Event event;
    const Uint8* keystate;
    int timerMS = SDL_GetTicks();
    int deltaTimeMS = 0;

    //======================================================================================
    // Create Bot
    //======================================================================================
    cBotBase* pBot = new cBotRandom();
    pBot->SetCurrent(10, 20, gLevel);
	
    //======================================================================================
    //Main loop
    //======================================================================================
    while(!gQuit)
	{   
        //======================================================================================
        //Poll events for quit
        //======================================================================================
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                gQuit = true;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                case SDLK_q:
                    gQuit = true;
                    break;
                }
                break;
            }
        }
        
        //======================================================================================
        //Keyboard input for target control 
        //======================================================================================
        keystate = SDL_GetKeyboardState(NULL);
        int offsetX = 0;
        int offsetY = 0;
        if (keystate[SDL_SCANCODE_UP]) offsetY -= 1;
        if (keystate[SDL_SCANCODE_DOWN]) offsetY += 1;
        if (keystate[SDL_SCANCODE_LEFT]) offsetX -= 1;
        if (keystate[SDL_SCANCODE_RIGHT]) offsetX += 1;

		static bool p_down = false;
        if (keystate[SDL_SCANCODE_P])
        {
            if (!p_down)
            {
                gAStar.Build(*pBot, heuristic);
                p_down = true;
            }
        }

        if ((offsetX != 0) || (offsetY != 0))
        {
            gTarget.SetNext((gTarget.PositionX() + offsetX), (gTarget.PositionY() + offsetY), gLevel);
        }

        //======================================================================================
        //Start render for this frame
        //======================================================================================
        SDL_SetRenderDrawColor(gMainRenderer, 200, 200, 255, 255);
        SDL_RenderClear(gMainRenderer);

        //======================================================================================
        //Compute time in miliseconds of last update cycle
        //======================================================================================
        int newFrameTimeMS = SDL_GetTicks();
        deltaTimeMS = newFrameTimeMS - timerMS;
        if (deltaTimeMS < 0) deltaTimeMS = 0;
        timerMS = newFrameTimeMS;

        //======================================================================================
        //Update moving objects
        //======================================================================================
        gTarget.Update(deltaTimeMS);
        pBot->Update(deltaTimeMS);

        //======================================================================================
        //Draw the level grid, then target, then bot
        //======================================================================================
        gLevel.Draw();
        gTarget.Draw(targetTexture);
        pBot->Draw(botTexture);

        //======================================================================================
        //Finalise render for this frame
        //======================================================================================
        SDL_RenderPresent(gMainRenderer);
	}

    //======================================================================================
    //Clean up
    //======================================================================================
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
	return 0;
}

