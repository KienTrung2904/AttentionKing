#pragma once
#ifndef OBJECT_H
#define OBJECT_H
// Custom library
#include "utils.h"
#include "gallery.h"
#include "constants.h"

// SDL2 library
#include <SDL.h>
#include <SDL_mixer.h>
// Standard library
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

// Random device
SDL_Texture* loadTextureFromFile(std::string headpath, std::string path, SDL_Renderer*& Renderer);
const int speed = 3;
class RandomGenerator {
private:
    std::mt19937_64 rng;
public:
    RandomGenerator() {}
    RandomGenerator(unsigned seed) { rng.seed(seed); }
    int randomInteger(int l, int r) { return rng() % (r - l + 1) + l; }
    int indexRandom(int i) { return rng() % i; }
};



enum LevelType {
    EASY = 3,
    MEDIUM,
    HARD,
    TOTAL_LEVEL
};

enum Color {
    BLACK = 0,
    PINK,
    GREEN,
    YELLOW,
    BLUE,
    TOTAL_COLOR
};
Color getColor(std::string s);
enum Direction {
    TO_UP = 0,
    TO_DOWN,
    TO_RIGHT,
    TO_LEFT,
    TOTAL_DIRECTION
};
Direction getDirection(std::string s);
enum SwitchType {
    NO_CHANGE_DIRECTION = 0,
    CHANGE_DIRECTION,
    TOTAL_SWITCH_TYPE
};

class Rail {
private:
    std::string nameRail;
    SDL_Texture* railText;
    SDL_Rect railRect;
    std::string headpath = "rail";
public:
    Rail() {}
    std::string getNameRail() { return nameRail; }
    SDL_Rect getRect() { return railRect; }
    Rail(int _left, int _top, std::string _nameRail, SDL_Renderer*& Renderer);
    void renderRail(SDL_Renderer*& Renderer);
};
SDL_Rect findPos(std::string a, std::string b, SDL_Rect Rect_a, SDL_Renderer*& Renderer);
class Switch {
private:
    SDL_Texture* currenSwitch;
    SDL_Texture* noChangeDirectionSwitch;
    SDL_Texture* changeDirectionSwitch;
    SDL_Rect switchRect;
    SwitchType currentSwitchType;
    std::string headpath = "switch";
    //  SwitchType nextSwitchType;
public:
    Switch() {};
    Switch(int _left, int _top, std::string switch1, std::string switch2, SDL_Renderer*& Renderer);
    bool isChangeDiretion(SwitchType type) { return type == CHANGE_DIRECTION; }
    SDL_Rect getSwitchRect() { return switchRect; }
    SwitchType getSwitchType() { return currentSwitchType; }
    bool isInSwitch(int mouseX, int mouseY) {
        return mouseX >= switchRect.x && mouseX <= switchRect.x + switchRect.w &&
            mouseY >= switchRect.y && mouseY <= switchRect.y + switchRect.h
            ;
    }
    void updateSwitch();
    void renderSwitch(SDL_Renderer*& Renderer);
};
class Tunnel {
private:
    SDL_Texture* tunnelText;
    SDL_Rect tunnelRect;
    std::string headpath = "tunnel";
public:
    Tunnel() {}
    Tunnel(int _left, int _top, std::string nameTunnel, SDL_Renderer*& Renderer);
    void renderTunnel(SDL_Renderer*& Renderer);
};

class Station {
private:
    SDL_Texture* stationText;
    SDL_Rect stationRect;
    Color stationColor;
    Direction directionStation;
    std::string headpath = "station";

public:
    Station() {};
    Station(int _left, int _top, int _width, int _height, std::string nameStation, SDL_Renderer*& Renderer);
    // bool isTraincomein(Train train);
    Color getSationColor() { return stationColor; }
    SDL_Rect getStationRect() { return stationRect; }
    void renderStation(SDL_Renderer*& Renderer);
};
class Train {
private:
    LevelType Level;
    SDL_Texture* trainText[TOTAL_DIRECTION];
    SDL_Texture* currentTrainText;
    SDL_Rect trainRect[TOTAL_DIRECTION];
    SDL_Rect currentTrainRect;
    Direction trainDirection;
    int currentRoute, currentPart, fork, correct;
    bool endPart, endRoute, endMove;
    std::vector <Color> colorStation;
    std::vector <std::vector<std::pair<int, int>>> startPos;
    std::vector <std::vector<std::pair<int, int>>> endPos;
    std::vector <std::vector<Direction>> direction;
    std::vector <bool> canEndMove;
    // Direction NextDirection;
    Color trainColor;
    std::string headpath = "train";
public:
    Train() {}
    Train(int _left, int _top, std::string nameTrain, LevelType _level,
        std::string pathRoad, SDL_Renderer*& Renderer);
    void updateDirection(std::string _direction);
    void updateTrain(int x, int y, Direction _direction) {
        currentTrainText = trainText[_direction];
        currentTrainRect = trainRect[_direction];
        currentTrainRect.x = x;
        currentTrainRect.y = y;
    }
    SDL_Texture* getCurrentTrainText() { return currentTrainText; }
    void updateTrainRect(SDL_Rect gRect) { currentTrainRect = gRect; }
    Color getTrainColor() { return trainColor; }
    SDL_Rect getCurrentTrainRect() { return currentTrainRect; }
    void renderTrain(SDL_Renderer*& Renderer, std::vector <Switch> switchList,
        int& count, int& inStation);
};

class Map {
private:
    LevelType Level;
    std::vector <std::vector<Rail>> railList;
    std::vector <std::vector<std::string>> nameRailList;
    std::vector <std::vector<SDL_Rect>> railRectList;
    std::vector <std::string> cornerNameList;
    std::vector <SDL_Rect> cornerRectList;
    std::vector <int> numOfRails;
    int sumOfTrains, correctTrains, trainAppeared;
    Tunnel tunnel;
    Train train;
    int res;
    std::vector <Train> allTrain;
    std::vector <std::string> nameTrain;
    std::vector <Station> stationList;
    std::vector <Switch> switchList;
    std::vector <Color> colorTrain;
public:
    Map() {};
    Map(SDL_Renderer*& Renderer, std::string path, LevelType _level, std::string pathRoad, std::string pathTrain);
    void renderRail(SDL_Renderer*& Renderer);
    void renderStation(SDL_Renderer*& Renderer);
    void renderTunnel(SDL_Renderer*& Renderer);
    void renderSwitchList(SDL_Renderer*& Renderer);
    void updateSwitch();
    int getTrainAppeared() { return trainAppeared; }
    int getCorrectTrain() { return correctTrains; }
    int getNumOfTrain() { return allTrain.size(); }
    SDL_Rect getChoosenSwitch(int mouseX, int mouseY, SDL_Renderer*& Renderer);
    void renderAllTrains(SDL_Renderer*& Renderer);
    void update();
    bool isWin() { return res >= allTrain.size() - 4; }

};
enum SoundState {
    PLAYING = 0,
    MUTING,
    TOTAL_SOUND
};
class playSound {
private: 
    Mix_Chunk* soundTrain;
    SDL_Texture* soundText[TOTAL_SOUND];
    SDL_Texture* currentSoundText;
    SDL_Rect currenSoundRect;
    SDL_Rect soundRect[TOTAL_SOUND];
    SoundState currentState;
    std::string headpath = "sound";
public:
    playSound() {};
    playSound(std::string path, SDL_Renderer*& Renderer);
    void play() {
        if (currentState == PLAYING) Mix_PlayChannel(-1, soundTrain, 0);
        else Mix_Volume(-1, 0);
    }
    void renderSound(SDL_Renderer*& Renderer);
    SoundState getSoundState() { return currentState; }
    void stop() { Mix_Volume(-1, 0); }
    void playAgain() {
        Mix_Volume(-1, MIX_MAX_VOLUME);
    }
    void update(int mouseX, int mouseY);
};
class Game {
private:
    LevelType level;
    Map map;
    GameState gameState = PLAYING_THE_GAME;
    playSound sound;

public:
    Game() {};
    Game(std::string pathSound, std::string pathMap, std::string pathRoad, std::string pathTrain, LevelType _level, SDL_Renderer*& Renderer);
    void renderGame(SDL_Renderer*& renderer, Gallery& gallery);
    void handleUserInput(SDL_Event e, SDL_Renderer*& Renderer);
    int getCorrectTrains() { return map.getCorrectTrain(); }
    int getTrainsAppeared() { return map.getTrainAppeared(); }
    int getNumOfTrain() { return map.getNumOfTrain(); }
    void updateGameState(GameState g) { gameState = g; }
    GameState getGameState() { return gameState; }
    void playMusic() {  sound.play(); }
    void stopMusic() { sound.stop(); }
 


};

#endif
