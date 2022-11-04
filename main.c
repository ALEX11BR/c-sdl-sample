#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

SDL_Window *window;
SDL_Renderer *render;

void mainLoop()
{
    SDL_Event event;
    SDL_WaitEvent(&event);

    if (event.type == SDL_QUIT)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();

        exit(0);
    }

    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, rand() % 255, rand() % 255, rand() % 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(render, NULL);

    SDL_RenderPresent(render);
}

int main()
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init error: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer error: %s", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (1)
    {
        mainLoop();
    }
#endif

    return 0;
}
