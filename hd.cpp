#include <iostream>
#include <SDL2/SDL.h>

void sdlError(const char* msg = NULL){
    if(msg != NULL){
        std::cerr << msg;
    } else {
        std::cerr << "SDL Error";
    }
    std::cerr << ": " << SDL_GetError() << std::endl;
    if(SDL_WasInit(SDL_INIT_EVERYTHING)!=0){
        std::cout << "SDL Shutdown.\n";
        SDL_Quit();
    }
    std::abort();
}
SDL_Window* mainWindow = NULL;
SDL_Surface* splash = NULL;
void loadResources(){
    mainWindow = SDL_CreateWindow("hd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, SDL_WINDOW_SHOWN);
    if(mainWindow != NULL){
        std::cout << "Window created.\n";
        splash = SDL_LoadBMP("splash.bmp");
        if(splash != NULL){
            std::cout << "Splash image loaded.\n";

        } else {
            sdlError("Load image failed");
        }
    } else {
        sdlError("SDL Create Window failed");
    }
}

void startup(){
    if(SDL_Init(SDL_INIT_EVERYTHING)==0){
        std::cout << "SDL Initialized.\n";
        loadResources();
    } else {
        sdlError("SDL Initialization failed");
    }
}


int main(int argc, char** argv){
    startup();
    return 0;
}