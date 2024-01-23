#include "My.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

int WIDTH = 640, HEIGHT = 480;
bool VSYNC = true;
uint8_t DISPLAY_MODE = 0;

int main(int argc, char *argv[]) {
    srand(time(0));
    /* Initializes the timer, audio, video, joystick,
  haptic, gamecontroller and events subsystems */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 0;
    }
    /* Create a window */
    SDL_Window *wind = SDL_CreateWindow("Bikoborbaaaaaa!", 0, 30, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP); // SDL_WINDOW_FULLSCREEN_DESKTOP
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
    double dt = 0;

    SDL_GetWindowSize(wind, &WIDTH, &HEIGHT);
    TTF_Init();
    SDL_Texture *floor = IMG_LoadTexture(rend, "media/floor.png");

    std::cout << WIDTH << " " << HEIGHT << std::endl;

    /* Main loop */
    bool running = true, inArena = false;
    int menu = 0, level = -1, tmpi = 0;
    std::string username = "", tmps;
    Minotaur mino(rend);
    SDL_Event event;
    BulletList bullets(rend);
    BullfighterList fighters(rend);
    ArenaList arenas(rend);
    Screen deathSc, menuSc, settingsSc, usernameSc, highscoreSC, creditsScreen, howToPlayScreen;
    Text FPS(WIDTH - 100, 10, 0, 0), levelText(HEIGHT / 2, 0, 0);
    Replay rep;
    Stat arenaStat(rend, "media/arena.png", WIDTH / 2 - 100, 20), barnStat(rend, "media/barn.png", WIDTH / 2 + 100, 20);
    deathSc.makeDeath(rend, mino);
    menuSc.makeMenu(rend);
    settingsSc.makeSettings(rend);
    usernameSc.makeUsername(rend);
    highscoreSC.makeHighscore(rend);
    creditsScreen.makeCredits(rend);
    howToPlayScreen.makeHowToPlay(rend);

    Keyb k;
    while (running) {
        dt = microseconds.count() * 0.000001;
        switch (menu) {
        case 0:
            menu = menuSc.menu(event, k, rend);
            continue;
        case -1:
            return 1;
        case 1:
            if (!username.length()) {
                if (usernameSc.username(event, k, rend, username) == -1)
                    running = false;
                rep.clear();
            }
            break;
        case 2:
            menu = highscoreSC.dis1(event, k, rend);
            continue;
        case 3:
            menu = rep.play(event, k, rend);
            continue;
        case 4:
            menu = -2;
            loadSave(rend, mino, fighters, arenas, username, level, barnStat);
            break;
        case 5:
            menu = settingsSc.settings(event, k, rend, wind, mino, menuSc, deathSc);
            continue;
        case 6:
            menu = howToPlayScreen.dis1(event, k, rend);
            continue;
        case 7:
            menu = creditsScreen.dis1(event, k, rend);
            continue;
        default:
            break;
        }

        while (SDL_PollEvent(&event))
            event.type == SDL_QUIT ? quit(running) : k.update(event);
        rep.s(k);

        if (k.get(SDL_SCANCODE_ESCAPE)) running = false;
        if (k.get(SDL_SCANCODE_W) || k.get(SDL_SCANCODE_S) || k.get(SDL_SCANCODE_D) || k.get(SDL_SCANCODE_A)) mino.move(k, MSPEED, dt);
        if (k.get(SDL_SCANCODE_SPACE)) mino.shoot(dt);
        if (k.get(SDL_SCANCODE_KP_1) || k.get(SDL_SCANCODE_1)) fighters.push(rand() % WIDTH, rand() % HEIGHT);
        if (k.get(SDL_SCANCODE_KP_2) || k.get(SDL_SCANCODE_2)) arenas.push(rand() % (WIDTH - 128), rand() % (HEIGHT - 128), rend);

        fighters.update(mino, dt);
        fighters.chk(mino, dt);
        switch (arenas.chk(rend, mino, k, inArena)) {
        case 1:
            barnStat.updateCount(1);
            break;
        case 4:
            quit(running);
            break;
        default:
            break;
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, floor, NULL, NULL);
        arenas.display(rend, mino);
        fighters.display(rend);
        mino.display(rend);

        barnStat.display(rend);
        FPS.display(rend, 1 / dt);
        arenaStat.setCount(arenas.size);
        arenaStat.display(rend);
        // level generation
        if (!fighters.size && !arenas.size) {
            tmpi++;
            tmps = "level ";
            tmps += ::std::to_string(level + 2);
            levelText.display(rend, tmps);
            if (tmpi > 500) {
                level++;
                tmpi = rand() % 30 + 10;
                for (int i = 0; i < (level < 5 ? levels[level][0] : tmpi); i++)
                    fighters.push(rand() % WIDTH, rand() % HEIGHT);
                for (int i = 0; i < (level < 5 ? levels[level][1] : tmpi); i++)
                    arenas.push(rand() % (WIDTH - 128), rand() % (HEIGHT - 128), (level < 5 ? levels[level][2] : tmpi), rend);
                tmpi = 0;
            }
        }
        if (!mino.getHealth()) {
            if (deathSc.death(event, k, rend)) {
                writeToHighscores(username, mino.getScore());
                mino.revive();
                mino.resetScore();
                fighters.destroy();
                arenas.destroy();
                level = -1;
            } else
                quit(running);
        }
        SDL_RenderPresent(rend);
        // SDL_Delay(1000 / FPS);
        start = std::chrono::high_resolution_clock::now();
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(start - stop);
        stop = start;
    }
    /* Release resources */
    if (dt != 0) {
        rep.save();
        writeToHighscores(username, mino.getScore());
        saveSave(mino, fighters, arenas, username, level, barnStat);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);
    SDL_Quit();
    return 0;
}
