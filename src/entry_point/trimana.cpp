#include <iostream>
#include <SDL3/SDL.h>

int main(int argc, char** argv){

    printf("%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, 0);
    return 0;
}