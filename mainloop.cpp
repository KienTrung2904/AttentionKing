#include "mainloop.h"
#include <string.h>
#include <SDL_mixer.h>
/*
enum GameState {
    STARTING_SCREEN = 0,
    CHOOSING_DIFFICULTY,
    PLAYING_THE_GAME,
    PAUSING_THE_GAME,
    LOSING_THE_GAME,
    WINNING_THE_GAME,
    RESTARTING_SCREEN,
    QUITTING_THE_GAME
};

*/

MainLoop::MainLoop(SDL_Renderer* renderer, Gallery& gallery) {

   
    mainMenu = loadMenuFromFile("data/menu/start.txt", renderer, gallery);
    howtoplayMenu = loadMenuFromFile("data/menu/howtoplay.txt", renderer, gallery);
    choosinglevelMenu = loadMenuFromFile("data/menu/choosinglevel.txt", renderer, gallery);
    pauseMenu = loadMenuFromFile("data/menu/pause.txt", renderer, gallery);
    difficultMenu = loadMenuFromFile("data/menu/difficulty.txt", renderer, gallery);
    confirmMenu = loadMenuFromFile("data/menu/confirm.txt", renderer, gallery);
    restartMenu = loadMenuFromFile("data/menu/restart.txt", renderer, gallery);

    pathSound = "data/sound/data.txt";

    map[EASY] = "data/map/easy.txt";
    map[MEDIUM] = "data/map/medium.txt";
    map[HARD] = "data/map/hard.txt";

    road[EASY] = "data/route/easy.txt";
    road[MEDIUM] = "data/route/medium.txt";
    road[HARD] = "data/route/hard.txt";

    trainList[EASY] = "data/train_list/easy.txt";
    trainList[MEDIUM] = "data/train_list/medium.txt";
    trainList[HARD] = "data/train_list/hard.txt";

    std::ifstream textboxLoader("data/textbox/data.txt");
    if (!textboxLoader.is_open()) {
        logError(std::cout, "Textbox file loading fail", true);
    }
    numOfCorrectTrain = createTextboxFromFile(textboxLoader, BUTTON);
    background = Background();

    gameState = STARTING_SCREEN;
}

void MainLoop::renderGame(SDL_Renderer*& renderer, Gallery& gallery, int mouseX, int mouseY) {
    // std::cout << "Mainloop rendering..." << std::endl;
    background.renderBackground(renderer, gallery);
    switch (gameState) {
    case STARTING_SCREEN: {
        mainMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }
    case HOW_TO_PLAY: {
        howtoplayMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    case CHOOSING_DIFFICULTY: {
        difficultMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    case PLAYING_THE_GAME: {
        game.renderGame(renderer, gallery);
        numOfCorrectTrain.renderTextBox(renderer, gallery);
        numOfCorrectTrain.updateText(std::to_string(game.getCorrectTrains()));
        // std::cout << SDL_GetError() << std::endl;  
        break;
    }

    case PAUSING_THE_GAME: {
        //game.renderGame(renderer, gallery);
        pauseMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    case WINNING_THE_GAME: {
        game.renderGame(renderer, gallery);
        background.renderBackground(renderer, gallery);
        break;
    }

    case LOSING_THE_GAME: {
        game.renderGame(renderer, gallery);
        background.renderBackground(renderer, gallery);
        break;
    }

    case RESTARTING_SCREEN: {
        restartMenu.renderMenu(renderer, gallery, mouseX, mouseY);
        break;
    }

    }
}

void MainLoop::handleUserInput(SDL_Event e, SDL_Renderer*& renderer, Gallery gallery) {
   
    switch (e.type) {
    case SDL_QUIT: {
        updateGameState(QUITTING_THE_GAME);
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        int mouseX = e.button.x, mouseY = e.button.y;
        switch (gameState) {
        case STARTING_SCREEN: {
            std::string clickedButton = mainMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "none") {
                logError(std::cout, "Clicking random thing in main menu", false);
            }
            else if (clickedButton == "play") {
                updateGameState(CHOOSING_DIFFICULTY);
                background.setBackgroundState(GAME_BACKGROUND);
            }
            else if (clickedButton == "how to play") {
                updateGameState(HOW_TO_PLAY);
                background.setBackgroundState(HOWTOPLAY_BACKGROUND);
            }
            else if (clickedButton == "quit") {
                updateGameState(QUITTING_THE_GAME);
            }
            else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in main menu: " + clickedButton, false);
            }
            break;
        }
        case HOW_TO_PLAY: {
            std::string clickedButton = howtoplayMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "back")
            {
                updateGameState(STARTING_SCREEN);
                background.setBackgroundState(MAIN_MENU);
            }
            else if (clickedButton == "play")
            {
                updateGameState(CHOOSING_DIFFICULTY);
                background.setBackgroundState(GAME_BACKGROUND);
            }
        }
        case CHOOSING_DIFFICULTY: {
            std::string clickedButton = difficultMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "none") {
                logError(std::cout, "Clicking random thing in difficulty menu", false);
            }
            else if (clickedButton == "easy") {
                game = Game(pathSound, map[EASY], road[EASY], trainList[EASY], EASY, renderer);
                updateGameState(PLAYING_THE_GAME);
                background.setBackgroundState(GAME_BACKGROUND);
                game.playMusic();
                numOfCorrectTrain.updateText(std::to_string(game.getCorrectTrains()) + " correct of " + std::to_string(game.getTrainsAppeared()));
                // background.setBackgroundState(GAME_BACKGROUND);

            }
            else if (clickedButton == "medium") {
                game = Game(pathSound, map[MEDIUM], road[MEDIUM], trainList[MEDIUM], MEDIUM, renderer);
                updateGameState(PLAYING_THE_GAME);
                
                background.setBackgroundState(GAME_BACKGROUND);
                game.playMusic();
                numOfCorrectTrain.updateText(std::to_string(game.getCorrectTrains()) + "correct of " + std::to_string(game.getTrainsAppeared()));
            }
            else if (clickedButton == "hard") {
                game = Game(pathSound, map[HARD], road[HARD], trainList[HARD], HARD, renderer);
                updateGameState(PLAYING_THE_GAME);
               
                background.setBackgroundState(GAME_BACKGROUND);
                game.playMusic();
                numOfCorrectTrain.updateText(std::to_string(game.getCorrectTrains()) + "correct of " + std::to_string(game.getTrainsAppeared()));
            }
            else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in diff menu: " + clickedButton, false);
            }
            break;
        }

        case PLAYING_THE_GAME: {
            // std::cout << "Handling the input" << std::endl;
            game.handleUserInput(e, renderer);
            if (game.getGameState() == WINNING_THE_GAME) {
                // std::cout << 1 << std::endl;
                updateGameState(WINNING_THE_GAME);
                game.stopMusic();
                background.setBackgroundState(GAME_WINNING);
            }
            else if (game.getGameState() == LOSING_THE_GAME) {
                // std::cout << 2 << std::endl;
                updateGameState(LOSING_THE_GAME);
                game.stopMusic();
                background.setBackgroundState(GAME_LOSING);
            }
            // std::cout << "Finished" << std::endl;
            break;
        }

        case WINNING_THE_GAME: {
            // std::cout << 1 << std::endl;
            //updateGameState(RESTARTING_SCREEN);
            SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
            SDL_RenderClear(renderer);
            background.setBackgroundState(GAME_WINNING);
            SDL_Delay(500);
            background.setBackgroundState(MAIN_MENU);
            updateGameState(STARTING_SCREEN);
            break;
        }

        case LOSING_THE_GAME: {
            // std::cout << 2 << std::endl;
            //updateGameState(RESTARTING_SCREENN
            SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
            SDL_RenderClear(renderer);
            background.setBackgroundState(GAME_LOSING);
            SDL_Delay(500);
            background.setBackgroundState(MAIN_MENU);
            updateGameState(STARTING_SCREEN);
            break;
        }

        case RESTARTING_SCREEN: {
            std::string clickedButton = restartMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "restart") {
                updateGameState(CHOOSING_DIFFICULTY);
            }
            else if (clickedButton == "main menu") {
                updateGameState(STARTING_SCREEN);
            }
            else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in restart menu: " + clickedButton, false);
            }
            break;
        }

        case PAUSING_THE_GAME: {
            std::string clickedButton = pauseMenu.getPressedButton(mouseX, mouseY);
            if (clickedButton == "resume") {
                updateGameState(PLAYING_THE_GAME);
            }
            else if (clickedButton == "restart") {
                updateGameState(CHOOSING_DIFFICULTY);
            }
            else if (clickedButton == "quit") {
                updateGameState(STARTING_SCREEN);
            }
            else {
                logError(std::cout, "Maybe there are undefined behavior somewhere, clicked button in restart menu: " + clickedButton, false);
            }
            break;
        }

        default: {
            break;
        }


        }
        break;
    }

    case SDL_KEYDOWN: {
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: {
            if (gameState == PLAYING_THE_GAME) {
                updateGameState(PAUSING_THE_GAME);
            }
            else if (gameState == PAUSING_THE_GAME) {
                updateGameState(PLAYING_THE_GAME);
            }
            break;
        }

        default: {
            break;
        }

        }
    }

    default: {
        break;
    }

    }
}