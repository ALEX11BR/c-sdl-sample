#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MOVE_STEP 5

SDL_Window *window;
SDL_Renderer *render;

SDL_Rect redRect = {0, 0, 200, 200};

void QuitGame(void)
{
    SDL_DestroyWindow(window);
    SDL_Quit();

    exit(0);
}

void MainLoop(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            QuitGame();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                redRect.x -= MOVE_STEP;
                break;
            case SDLK_DOWN:
                redRect.y += MOVE_STEP;
                break;
            case SDLK_UP:
                redRect.y -= MOVE_STEP;
                break;
            case SDLK_RIGHT:
                redRect.x += MOVE_STEP;
                break;
            }
            break;
        }
    }

    SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(render, &redRect);

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

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer error: %s", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(MainLoop, 0, 1);
#else
    while (1)
    {
        MainLoop();
    }
#endif

    return 0;
}
