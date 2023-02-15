#include "keyboard.cpp"
#include "My.h"
#include <chrono>
#include <ctime>
#include <iostream>

#define FPS 60

int WIDTH = 640, HEIGHT = 480;

int main(int argc, char *argv[]) {
    srand(time(0));
    /* Initializes the timer, audio, video, joystick,
  haptic, gamecontroller and events subsystems */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 0;
    }
    /* Create a window */
    SDL_Window *wind = SDL_CreateWindow("Bikoborbaaaaaa!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP); // SDL_WINDOW_FULLSCREEN_DESKTOP
    if (!wind) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    /* Create a renderer */
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(wind, -1, render_flags);
    if (!rend) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(wind);
        SDL_Quit();
        return 0;
    }

    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now(), stop = start;
    std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    double dt;

    SDL_GetWindowSize(wind, &WIDTH, &HEIGHT);
    TTF_Init();
    SDL_Surface *image;
    image = SDL_LoadBMP("media/back1.bmp");
    SDL_Texture *floor = SDL_CreateTextureFromSurface(rend, image);
    SDL_FreeSurface(image);
    floor = IMG_LoadTexture(rend, "media/floor.png");

    /* Main loop */
    bool running = true;
    Minotaur mino(rend);
    SDL_Event event;
    BulletList bullets(rend);
    BullfighterList fighters(rend);
    ArenaList arenas(rend);
    Screen menuSc(rend);
    menuSc.makeM(mino);
    int menu = 0;

    Keyb k;

    while (running) {
        dt = microseconds.count() * 0.000001;
        /* Process events */
        while (SDL_PollEvent(&event))
            event.type == SDL_QUIT ? quit(running) : k.update(event);

        if (k.get(SDL_SCANCODE_ESCAPE)) running = false;
        if (k.get(SDL_SCANCODE_W) || k.get(SDL_SCANCODE_S) || k.get(SDL_SCANCODE_D) || k.get(SDL_SCANCODE_A)) mino.move(k, MSPEED);
        if (k.get(SDL_SCANCODE_SPACE)) mino.shoot(bullets);
        if (k.get(SDL_SCANCODE_KP_1)) fighters.push(rand() % WIDTH, rand() % HEIGHT);
        if (k.get(SDL_SCANCODE_KP_2)) arenas.push(rand() % WIDTH, rand() % HEIGHT);

        fighters.update(bullets);
        fighters.chk(bullets, mino);
        bullets.update();
        bullets.chk();
        arenas.chk(mino);
        mino.chk(bullets);

        if (!mino.getHealth())
            menu = 1;

        if (!menu) {
            /* Clear screen */
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderClear(rend);
            /* Draw to window and loop */
            SDL_RenderCopy(rend, floor, NULL, NULL);
            arenas.display(rend);
            fighters.display(rend);
            bullets.display(rend);
            mino.display(rend);
        } else {
            if (!menuSc.getDis())
                menuSc.display(rend);
            else if (k.get(SDL_SCANCODE_SPACE)) {
                mino.revive(menu);
                menuSc.setDis(0);
            }
        }
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS);
        start = std::chrono::high_resolution_clock::now();
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(start - stop);
        stop = start;
    }
    /* Release resources */
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);
    SDL_Quit();
    return 0;
}
