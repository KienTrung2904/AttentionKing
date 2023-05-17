#pragma once
// Custom library
#include "constants.h"
#include "utils.h"
#include "object.h"
#include "gallery.h"
#include "menu.h"

// SDL2 library

// Standard library
#include <utility>



class MainLoop {
private:
    Menu mainMenu, howtoplayMenu, choosinglevelMenu, difficultMenu, pauseMenu, confirmMenu, restartMenu;
    Background background;
    Game game;
    GameState gameState;
    std::string pathSound;
    std::string map[TOTAL_LEVEL];
    std::string road[TOTAL_LEVEL];
    std::string trainList[TOTAL_LEVEL];
    Textbox numOfCorrectTrain;

public:
    MainLoop(SDL_Renderer* renderer, Gallery& gallery);
    void updateGameState(GameState state) { gameState = state; }
    void renderGame(SDL_Renderer*& renderer, Gallery& gallery, int mouseX, int mouseY);
    void handleUserInput(SDL_Event e, SDL_Renderer*& renderer, Gallery gallery);
    GameState getGameState() { return gameState; }
};
