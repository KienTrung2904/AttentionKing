// Custom library
#include "object.h"
#include <fstream>
#include <vector>
#include <string>
// SDL2 library

// Standard library
SDL_Rect findPos(std::string a, std::string b, SDL_Rect Rect_a, SDL_Renderer*& Renderer) {
    int w = 0, h = 0;
    std::string headpath = "rail/";
    SDL_Texture* getTexture = loadTextureFromFile(headpath, b, Renderer);
    SDL_QueryTexture(getTexture, NULL, NULL, &w, &h);
    int ka = 1, kb = -1;
    if (a[0] == '-') ka = -1;
    if (b[0] == '-') kb = 1;
    int x = 0, y = 0;
    if (a == b || a[1] == '6' || a[1] == '7' || b[1] == '6' || b[1] == '7') {
        if (a[1] == '0' || a[1] == '6') {
            y = Rect_a.y;
        }
        else {
            y = Rect_a.y + kb * h;
        }
        if (a[1] == '5' || a[1] == '7')
        {
            x = Rect_a.x;
        }
        else {
            if (ka == 1) {
                x = Rect_a.x + ka * Rect_a.w;
            }
            else {
                x = Rect_a.x + ka * w;
            }
        }
    }
    else {
        if (a[1] == '0') {
            if (b == "-2" || b == "+1") {
                y = Rect_a.y;
            }
            else {
                y = Rect_a.y - (h - Rect_a.h);
            }
            if (b == "+3" || b == "-2") {
                x = Rect_a.x - w;
            }
            else {
                x = Rect_a.x + Rect_a.w;
            }
        }
        else if (a[1] != '5' && b[1] == '0') {
            if (a == "+2" || a == "-3") {
                x = Rect_a.x + Rect_a.w;
            }
            else {
                x = Rect_a.x - w;
            }
            if (a == "+2" || a == "-1") {
                y = Rect_a.y;
            }
            else {
                y = Rect_a.y + (Rect_a.h - h);
            }
        }
        if (a[1] == '5') {
            if (b == "+2" || b == "-3") {
                x = Rect_a.x;
            }
            else {
                x = Rect_a.x - (w - Rect_a.w);
            }
            if (b == "+2" || b == "-1") {
                y = Rect_a.y - h;
            }
            else {
                y = Rect_a.y + Rect_a.h;
            }
        }
        else if (a[1] != '0' && b[1] == '5') {
            if (a == "+3" || a == "-2") {
                x = Rect_a.x;
            }
            else {
                x = Rect_a.x + (Rect_a.w - w);
            }
            if (a == "-2" || a == "+1") {
                y = Rect_a.y + Rect_a.h;
            }
            else {
                y = Rect_a.y - h;
            }
        }


    }
    SDL_Rect res = { x, y, w, h };
    return res;
}
Color getColor(std::string s) {
    int pos = s.find('_');
    if (pos == -1) {
        pos = s.length();
    }
    std::string color = s.substr(0, pos);
    if (color == "black") return BLACK;
    else
        if (color == "pink") return PINK;
        else
            if (color == "green") return GREEN;
            else
                if (color == "yellow") return YELLOW;
                else
                    if (color == "blue") return BLUE;
                    else
                        return TOTAL_COLOR;
}

Direction getDirection(std::string s)
{
    int pos = s.find('_');
    int length = s.length();
    std::string direction = s.substr(pos + 1, length - 1);
    if (direction == "toup") return TO_UP;
    else
        if (direction == "todown") return TO_DOWN;
        else
            if (direction == "toright") return TO_RIGHT;
            else
                if (direction == "toleft") return TO_LEFT;
                else
                    return TOTAL_DIRECTION;


}

SDL_Texture* loadTextureFromFile(std::string headpath, std::string path, SDL_Renderer*& Renderer)
{
    std::string link = "image/" + headpath + "/" + path + ".png";
    std::cout << link << ": ";
    SDL_Surface* loadedSurface = IMG_Load(link.c_str());
    if (loadedSurface == nullptr) { std::cout << "Could not load image!"; }
    else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
        SDL_Texture* mTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
        if (mTexture == NULL)
        {
            std::cout << "Could not load image!" << std::endl;
        }
        else { std::cout << "Load image success fully!" << std::endl; }
        SDL_FreeSurface(loadedSurface);
        return mTexture;
    }
    return NULL;

}

Rail::Rail(int _left, int _top, std::string _nameRail, SDL_Renderer*& Renderer)
{
    railText = loadTextureFromFile(headpath, _nameRail, Renderer);
    nameRail = _nameRail;
    int w, h;
    SDL_QueryTexture(railText, NULL, NULL, &w, &h);
    railRect = { _left, _top, w, h };
}
void Rail::renderRail(SDL_Renderer*& Renderer)
{
    SDL_RenderCopy(Renderer, railText, NULL, &railRect);
}

Switch::Switch(int _left, int _top, std::string nameswitch1, std::string nameswitch2, SDL_Renderer*& Renderer)
{
    // switchRect = { _left, _top, _width, _height };
    currentSwitchType = NO_CHANGE_DIRECTION;
    noChangeDirectionSwitch = loadTextureFromFile(headpath, nameswitch1, Renderer);
    changeDirectionSwitch = loadTextureFromFile(headpath, nameswitch2, Renderer);
    currenSwitch = noChangeDirectionSwitch;
    switchRect = { _left, _top, 77, 77 };

}
void Switch::updateSwitch()
{
    if (currentSwitchType == NO_CHANGE_DIRECTION) {
        currenSwitch = changeDirectionSwitch;
        currentSwitchType = CHANGE_DIRECTION;
    }
    else {
        currenSwitch = noChangeDirectionSwitch;
        currentSwitchType = NO_CHANGE_DIRECTION;
    }
}
void Switch::renderSwitch(SDL_Renderer*& Renderer)
{
    SDL_RenderCopy(Renderer, currenSwitch, NULL, &switchRect);
}

Tunnel::Tunnel(int _left, int _top, std::string nameTunnel, SDL_Renderer*& Renderer)
{
    tunnelText = loadTextureFromFile(headpath, nameTunnel, Renderer);
    int w, h;
    SDL_QueryTexture(tunnelText, NULL, NULL, &w, &h);
    tunnelRect = { _left, _top, w, h };
}
void Tunnel::renderTunnel(SDL_Renderer*& Renderer)
{
    SDL_RenderCopy(Renderer, tunnelText, NULL, &tunnelRect);
}
Station::Station(int _left, int _top, int _width, int _height, std::string nameStation, SDL_Renderer*& Renderer)
{
    stationText = loadTextureFromFile(headpath, nameStation, Renderer);
    stationRect = { _left, _top, _width, _height };
    // get Color Station
    stationColor = getColor(nameStation);
    // get Direction Station
    directionStation = getDirection(nameStation);
}
void Station::renderStation(SDL_Renderer*& Renderer)
{
    SDL_RenderCopy(Renderer, stationText, NULL, &stationRect);
}
void Train::updateDirection(std::string _direction) {
    trainDirection = getDirection(_direction);
    currentTrainText = trainText[trainDirection];
    currentTrainRect = trainRect[trainDirection];
}

Train::Train(int _left, int _top, std::string nameTrain, LevelType _level, std::string pathRoad, SDL_Renderer*& Renderer)
{
    Level = _level;
    currentRoute = 0;
    currentPart = 0;
    endPart = false;
    endRoute = false;
    endMove = false;
    fork = 0;
    std::ifstream file3(pathRoad);
    if (!file3.is_open()) {
        std::cout << "can not open file dataRoad! \n";
    }
    else {
        std::cout << "Open file dataRoad successfully \n";
    }
    Level = _level;
    direction.resize(2 * Level - 1);
    startPos.resize(2 * Level - 1);
    endPos.resize(2 * Level - 1);
    canEndMove.resize(2 * Level - 1);
    colorStation.resize(2 * Level - 1);
    int numOfCorner, startX, startY, endX, endY;
    std::string directionTrain, canEnd, color;
    for (int i = 0; i < 2 * Level - 1; i++) {
        file3 >> numOfCorner >> canEnd >> color;
        if (canEnd == "yes") canEndMove[i] = true; else canEndMove[i] = false;
        if (color != "no") {
            colorStation[i] = getColor(color);
        }
        std::cout << numOfCorner << std::endl;
        for (int j = 0; j < numOfCorner; j++) {
            file3 >> directionTrain >> startX >> startY >> endX >> endY;
            direction[i].push_back(getDirection(directionTrain));
            startPos[i].push_back(std::make_pair(startX, startY));
            endPos[i].push_back(std::make_pair(endX, endY));
            std::cout << direction[i][j] << " " << startX << " " << startY << " " << endX << " " << endY << std::endl;
        }
    }
    trainColor = getColor(nameTrain);
    trainDirection = direction[0][0];
    std::string d[4] = { "_toup", "_todown", "_toright", "_toleft" };
    int w, h;
    for (int i = 0; i < TOTAL_DIRECTION; i++) {
        trainText[i] = loadTextureFromFile(headpath, nameTrain + d[i], Renderer);
        SDL_QueryTexture(trainText[i], NULL, NULL, &w, &h);
        trainRect[i] = { _left, _top, w, h };
        std::cout << trainRect[i].w << " " << trainRect[i].h << std::endl;
        currentTrainText = trainText[trainDirection];
        currentTrainRect = trainRect[trainDirection];
    }
    std::cout << "Initial class Train successfully! \n";
}



void Train::renderTrain(SDL_Renderer*& Renderer, std::vector <Switch> switchList, int& count, int& inStation ) {
    if (!endMove) {

        SDL_Rect gRect = currentTrainRect;
        switch (direction[currentRoute][currentPart]) {
        case TO_UP: {
            if (gRect.y >= endPos[currentRoute][currentPart].second) {
                gRect.y -= speed;
                SDL_RenderCopy(Renderer, currentTrainText, NULL, &gRect);
            }
            else {
                endPart = true;
                if (currentPart == direction[currentRoute].size() - 1) {
                    endRoute = true;
                }
            }
            break;
        }
        case TO_DOWN: {
            if (gRect.y <= endPos[currentRoute][currentPart].second) {
                gRect.y += speed;
                SDL_RenderCopy(Renderer, currentTrainText, NULL, &gRect);
            }
            else {
                endPart = true;
                if (currentPart == direction[currentRoute].size() - 1) {
                    endRoute = true;
                }
            }
            break;
        }
        case TO_RIGHT: {
            if (gRect.x <= endPos[currentRoute][currentPart].first) {
                gRect.x += speed;
                SDL_RenderCopy(Renderer, currentTrainText, NULL, &gRect);
            }
            else {
                endPart = true;
                if (currentPart == direction[currentRoute].size() - 1) {
                    endRoute = true;
                }
            }
            break;
        }
        case TO_LEFT: {
            if (gRect.x >= endPos[currentRoute][currentPart].first) {
                gRect.x -= speed;
                SDL_RenderCopy(Renderer, currentTrainText, NULL, &gRect);
            }
            else {
                endPart = true;
                if (currentPart == direction[currentRoute].size() - 1) {
                    endRoute = true;
                }
            }
            break;
        }
        default:
            break;
        }
        if (endPart) {
            if (!endRoute) {
                currentPart++;
                endPart = false;
            }
            else {
                if (!canEndMove[currentRoute]) {
                    if (switchList[fork].getSwitchType() == NO_CHANGE_DIRECTION) {
                        std::cout << switchList[fork].getSwitchRect().x << " " << switchList[fork].getSwitchRect().y << " ";
                        currentRoute++;
                    }
                    else {
                        if (Level == EASY) {
                            currentRoute += 2;
                        }
                        else if (Level == MEDIUM) {
                           
                            if (currentRoute == 0) {
                                fork++;
                                currentRoute += 3;
                            }
                            else if (currentRoute == 3) {
                                std::cout << "check!";
                                currentRoute += 3;
                            }
                            else if (currentRoute == 1) currentRoute += 4;
                            else currentRoute++;
                        }
                        else {
                            if (currentRoute == 0) {
                                currentRoute = 3;
                                fork++;
                            }
                            else if (currentRoute == 1) currentRoute = 6;
                            else currentRoute += 4;
                        }

                    }
                    fork++;
                    currentPart = 0;
                    endPart = false;
                    endRoute = false;
                }
                else {
                    inStation++;
                    if (colorStation[currentRoute] == trainColor) {
                        count++;
                    }
                    endMove = true;
                }
            }
            if (!endMove) {
                currentTrainText = trainText[direction[currentRoute][currentPart]];
                currentTrainRect = trainRect[direction[currentRoute][currentPart]];
                currentTrainRect.x = startPos[currentRoute][currentPart].first;
                currentTrainRect.y = startPos[currentRoute][currentPart].second;
            }
        }
        else {
            currentTrainRect = gRect;
        }

    }

}


Map::Map(SDL_Renderer*& Renderer, std::string path, LevelType _level, std::string pathRoad, std::string pathTrain)
{
    //  read list of trains
    Level = _level;
    correctTrains = 0;
    trainAppeared = 0;
    std::ifstream file2(pathTrain);
    if (!file2.is_open()) {
        std::cout << "can not open file dataTrainList! \n";
    }
    else {
        std::cout << "Open file dataTrainList successfully \n";
    }
    int xFirst, yFirst;
    std::string direct;
    file2 >> xFirst >> yFirst >> direct >> sumOfTrains;
    colorTrain.resize(sumOfTrains);
    std::string name;
    for (int i = 0; i < sumOfTrains; i++) {
        file2 >> name;
        colorTrain[i] = getColor(name);
        nameTrain.push_back(name);
        allTrain.push_back(Train(xFirst, yFirst, nameTrain[i], Level, pathRoad, Renderer));
    }

    railList.resize(Level);
    // numOfRails.resize(Level);
   //  nameRailList.resize(Level);
    railRectList.resize(Level);
    std::ifstream file(path);

    /*
    map 3 :
       tunnel_left 525 321 // name and pos of tunnel
       471 400 // pos of first rail neares tunnel
        11 // number of Rails
        -0 -0 -0 -0 -0 +3 +5 +7 +5 +2 +0
        11 // number of Rails
        +2 +0 +0 -4 +5 +2 +0 +6 +0 +1 -5
        2 // number of Rails
        +1 -5
        // name and rect of station
        pink_todown 560 233 68 94
        black_todown 432 233 68 94
        +5 +2 107 263 // name of two types of switch and their (x;y)
        +0 +1 425 137
    */

    // Declare here
    std::string nameOfTunnel, nameOfStation, nameOfSwitch1, nameOfSwitch2;
    int xTunnel, yTunnel, xFirstRail, yFirstRail, xStation, yStation, wStation, hStation, xSwitch, ySwitch;

    // read Tunnel
    file >> nameOfTunnel >> xTunnel >> yTunnel;
    tunnel = Tunnel(xTunnel, yTunnel, nameOfTunnel, Renderer);

    // read x and y of first Rail
    file >> xFirstRail >> yFirstRail;

    //read Rails
    int numOfRailEachRoad, sumOfRails = 0, numOfCorners = 0;
    std::string nameOfRail;
    // start to read from file

    res = 0;
    int n;
    std::string a;
    std::vector <std::string> tmp;
    for (int i = 0; i < Level; i++) {
        file >> n;
        for (int j = 0; j < n; j++) {
            file >> a;
            tmp.push_back(a);
            if (a[1] == '6' || a[1] == '7') {
            }
            // fout << tmp[tmp.size() -1] << " ";
        }
        railRectList[i].resize(tmp.size());
        railList[i].resize(tmp.size());
        nameRailList.push_back(tmp);
        tmp.clear();
        //fout << std::endl;
    }
    railList[0][0] = Rail(xFirstRail, yFirstRail, nameRailList[0][0], Renderer);
    railRectList[0][0] = railList[0][0].getRect();
    for (int i = 0; i < Level; i++) {
        for (int k = 0; k < nameRailList[i].size(); k++) {

            if (nameRailList[i][k][1] == '6' || nameRailList[i][k][1] == '7') {

                cornerNameList.push_back(nameRailList[i][k - 1]); // save corner name
                cornerRectList.push_back(railRectList[i][k - 1]);

            }
            if (k != 0) {
                SDL_Rect getRect = findPos(nameRailList[i][k - 1], nameRailList[i][k], railRectList[i][k - 1], Renderer);
                int getX, getY;
                getX = getRect.x;
                getY = getRect.y;
                railList[i][k] = Rail(getX, getY, nameRailList[i][k], Renderer);
                railRectList[i][k] = { getX, getY, railList[i][k].getRect().w, railList[i][k].getRect().h };
            }
            else {
                if (i != 0) {
                    SDL_Rect getRect = findPos(cornerNameList[numOfCorners], nameRailList[i][k], cornerRectList[numOfCorners], Renderer);
                    numOfCorners++;
                    int getX, getY;
                    getX = getRect.x;
                    getY = getRect.y;
                    railList[i][k] = Rail(getX, getY, nameRailList[i][k], Renderer);
                    railRectList[i][k] = { getX, getY, railList[i][k].getRect().w, railList[i][k].getRect().h };
                }
            }
        }
    }

    for (int i = 0; i < Level; i++) {
        file >> nameOfStation >> xStation >> yStation >> wStation >> hStation;
        // fout << nameOfStation <<" " << xStation << " " <<  yStation << " " <<  wStation << " " << hStation << std::endl;
        stationList.push_back(Station(xStation, yStation, wStation, hStation, nameOfStation, Renderer));
    }
    // nameDefaut, 
    for (int i = 0; i < Level - 1; i++) {
        file >> nameOfSwitch1 >> nameOfSwitch2 >> xSwitch >> ySwitch;
        //  fout << nameOfSwitch1 <<" " << nameOfSwitch2  << " " << xSwitch << " " << ySwitch << std::endl;
        switchList.push_back(Switch(xSwitch, ySwitch, nameOfSwitch1, nameOfSwitch2, Renderer));
    }


}



void Map::renderRail(SDL_Renderer*& Renderer) {
    for (int i = 0; i < Level; i++) {
        for (int j = 0; j < railList[i].size(); j++) {
            railList[i][j].renderRail(Renderer);
        }
    }

}
void Map::renderStation(SDL_Renderer*& Renderer) {
    for (int i = 0; i < stationList.size(); i++) {
        stationList[i].renderStation(Renderer);
    }
}
void Map::renderTunnel(SDL_Renderer*& Renderer) {
    tunnel.renderTunnel(Renderer);
}
void Map::renderSwitchList(SDL_Renderer*& Renderer) {
    for (int i = 0; i < switchList.size(); i++) {
        switchList[i].renderSwitch(Renderer);
    }
}
/*
    updateTrainRect(SDL_Rect gRect)
*/

SDL_Rect Map::getChoosenSwitch(int mouseX, int mouseY, SDL_Renderer*& Renderer) {
    SDL_Rect gRect = { 0, 0, 0, 0 };
    for (int i = 0; i < Level - 1; i++) {
        if (switchList[i].isInSwitch(mouseX, mouseY)) {
            gRect = switchList[i].getSwitchRect();
            std::cout << switchList[i].getSwitchType() << std::endl;
            switchList[i].updateSwitch();
            switchList[i].renderSwitch(Renderer);
        }
    }
    return gRect;
}

void Map::renderAllTrains(SDL_Renderer*& Renderer) {
    int startTime = SDL_GetTicks(); // thời điểm bắt đầu chạy
    // std::cout << allTrain.size() << std::endl;
    for (int i = 0; i < allTrain.size(); i++) {
        //int start_time = startTime + 2000 * i; // thời điểm bắt đầu của Train thứ i
        int current_time = SDL_GetTicks(); // thời điểm hiện tại

        // if (current_time >= start_time) {

        if (current_time > 3000 * i) {
            allTrain[i].renderTrain(Renderer, switchList, correctTrains, trainAppeared); // render Train thứ i
        }
        // }
        // std::cout << correctTrains << '\n';
        if (i == allTrain.size()) res = correctTrains;

    }


}

playSound::playSound(std::string path, SDL_Renderer*& Renderer) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        // Xử lý lỗi khi khởi tạo SDL
        std::cout << "Could not initialize Sound! \n";
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "Failed to initialize mixer! \n";
    }
    
    std::ifstream file(path);
    int x, y, w, h;
    std::string name;
    file >> name;
    soundTrain = Mix_LoadWAV((headpath + "/" + name + ".wav").c_str());
    if (soundTrain == NULL) {
        std::cout << "Failed to open ";
    }
    file >> x >> y;
    for (int i = 0; i < TOTAL_SOUND; i++) {
        file >> name >> w >> h;
        soundText[i] = loadTextureFromFile(headpath, name, Renderer);
        soundRect[i] = {x, y, w ,h};
    }
    currentState = PLAYING;
    currentSoundText = soundText[currentState];
    currenSoundRect = soundRect[currentState];

}

void playSound::update(int mouseX, int mouseY) {
    if (mouseX >= soundRect[currentState].x && mouseX <= soundRect[currentState].x +
        soundRect[currentState].w && mouseY >= soundRect[currentState].y && mouseY <=
        soundRect[currentState].y + soundRect[currentState].h) {
        if (currentState == PLAYING) { currentState = MUTING; }
        else { currentState = PLAYING; }
        std::cout << currentState << std::endl;
        currentSoundText = soundText[currentState];
        currenSoundRect = soundRect[currentState];
    }  
}
void playSound::renderSound(SDL_Renderer*& Renderer) {
    SDL_RenderCopy(Renderer, currentSoundText, NULL, &currenSoundRect);
}

Game::Game(std::string pathSound, std::string path, std::string pathRoad, std::string pathTrain, LevelType _level, SDL_Renderer*& Renderer) {
    level = _level;
    map = Map(Renderer, path, level, pathRoad, pathTrain);
    std::cout << "creat Map successfully \n ";

    std::cout << "create trainlist successfully \n";
    sound = playSound(pathSound, Renderer);
}

void Game::renderGame(SDL_Renderer*& Renderer, Gallery& gallery) {

    map.renderRail(Renderer);
    map.renderSwitchList(Renderer);
    map.renderAllTrains(Renderer);
    map.renderStation(Renderer);
    map.renderTunnel(Renderer);
    sound.renderSound(Renderer);
}

void Game::handleUserInput(SDL_Event e, SDL_Renderer*& Renderer) {
    /*
        Function to handle all user input:
        - Quit game.
        - Choose a cell.
    */
    if (map.getTrainAppeared() == map.getNumOfTrain()) {
        sound.stop();
        if (map.getCorrectTrain() >= map.getNumOfTrain() - 4) {
            updateGameState(WINNING_THE_GAME);
        }
        else updateGameState(LOSING_THE_GAME);
    }
    if (e.type == SDL_QUIT) {
        updateGameState(QUITTING_THE_GAME);
        return;
    }
    else {
        int mouseX = e.button.x, mouseY = e.button.y;
        SDL_Rect gRect = map.getChoosenSwitch(mouseX, mouseY, Renderer);
        SoundState old_state = sound.getSoundState();
        sound.update(mouseX, mouseY);   
        if (sound.getSoundState() != old_state) {
            if (old_state == PLAYING) {
                sound.stop();
            }
            else
            {
                sound.playAgain();
            }
        }
      
    }
    // std::cout << "Done!" << std::endl;

}