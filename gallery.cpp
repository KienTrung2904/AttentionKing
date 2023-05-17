// Custom library
#include "gallery.h"
#include "utils.h"

// SDL2 library

// Standard library
#include <string>
#include <algorithm>

Gallery::Gallery(SDL_Renderer* _renderer) {
    renderer = _renderer;
    loadFont("font/times.ttf");
    loadGamePictures();
}

Gallery::~Gallery() {
    std::vector <std::vector <SDL_Texture*> > tmp;
    pictures.swap(tmp);
}

void Gallery::loadFont(std::string path) {
    font = TTF_OpenFont(path.c_str(), 90);
    font2 = TTF_OpenFont(path.c_str(), 10);
    if (font == nullptr) {
        {
            logSDLError(std::cout, "Can not open font: " + path, true);
        }
    }
}

std::vector <SDL_Texture*> Gallery::loadTextureFromImage(std::string path,
    int numberOfFrame, std::string extension, SDL_BlendMode blendMode) {
    /*
        This function is used to load an animation series
        Input:
            Path (string): The path to the animation list
            numberOfFrame (int): Number of frame of the animation
        Output:
            animation (vector <SDL_Texture*>): The loaded animation of the object.
    */
    std::vector <SDL_Surface*> surfaceList(numberOfFrame);
    std::vector <SDL_Texture*> animation(numberOfFrame);
    std::cout << path << std::endl;
    for (int i = 0; i < numberOfFrame; i++) {
        surfaceList[i] = IMG_Load((path + "_" + std::to_string(i) + extension).c_str());
        SDL_SetColorKey(surfaceList[i], SDL_TRUE, SDL_MapRGB(surfaceList[i]->format, 0xFF, 0xFF, 0xFF));
        if (surfaceList[i] == nullptr) {
            logSDLError(std::cout, "Can not load image: " + path, true);
        }
        animation[i] = SDL_CreateTextureFromSurface(renderer, surfaceList[i]);
        SDL_SetTextureBlendMode(animation[i], blendMode);
    }

    return animation;
}

void Gallery::loadGamePictures() {

    pictures.push_back(loadTextureFromImage("image/game_win", 1, ".png", SDL_BLENDMODE_BLEND));
    pictures.push_back(loadTextureFromImage("image/game_over", 1, ".png", SDL_BLENDMODE_BLEND));
    pictures.push_back(loadTextureFromImage("image/button", 1, ".png", SDL_BLENDMODE_BLEND));
    pictures.push_back(loadTextureFromImage("image/menu_background", 1, ".png", SDL_BLENDMODE_BLEND));
    pictures.push_back(loadTextureFromImage("image/game_background", 1, ".png", SDL_BLENDMODE_BLEND));
    pictures.push_back(loadTextureFromImage("image/howtoplay_background", 1, ".png", SDL_BLENDMODE_BLEND));

}

SDL_Texture* Gallery::getFrame(PictureID obj, int& currentFrame) {
    if (currentFrame < 0) {
        logError(std::cout, "Invalid frame ", true);
    }
    currentFrame %= pictures[obj].size();
    return pictures[obj][currentFrame];
}

SDL_Texture* Gallery::loadTextureFromText(std::string textString, SDL_Color textColor) {
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, textString.c_str(), textColor);
    if (loadedSurface == nullptr) {
        logSDLError(std::cout, "Can not create surface from text: " + textString, true);
    }
    SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_free(loadedSurface);
    return loadedTexture;
}

SDL_Texture* Gallery::loadTextureFromText2(std::string textString, SDL_Color textColor) {
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(font2, textString.c_str(), textColor);
    if (loadedSurface == nullptr) {
        logSDLError(std::cout, "Can not create surface from text: " + textString, true);
    }
    SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_free(loadedSurface);
    return loadedTexture;
}
