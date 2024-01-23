Screen::Screen() {
    t.empty();
}
void Screen::renderBlendedBackground(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(rend, &r);
    SDL_RenderCopy(rend, NULL, NULL, &r);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
}
void Screen::display(SDL_Renderer *rend) { // za displayat use k mas v text vektorju
    renderBlendedBackground(rend);
    for (int i = 0; i < t.size(); i++) {
        t[i].display(rend);
    }
}
void Screen::display(SDL_Renderer *rend, int x) { // za diplayat sam en element v text vectorju
    t[x].display(rend);
}
void Screen::pushText(std::string st, int y, int w, int h) {
    Text te(-1, y, w, h, st);
    t.push_back(te);
}
void Screen::pushText(std::string st, int x, int y, int w, int h) {
    Text te(x, y, w, h, st);
    t.push_back(te);
}
void Screen::popText() {
    t.pop_back();
}
void Screen::clear() {
    t.clear();
    c.clear();
}
void Screen::makeDeath(SDL_Renderer *rend, Minotaur mino) {
    pushText(std::string("SCORE: ") + std::to_string(mino.getScore()), HEIGHT / 3, 0, 0);
    pushText("press space to continue...", 4 * HEIGHT / 5, 0, 0);
    image = IMG_LoadTexture(rend, "media/menu.png");
}
void Screen::makeMenu(SDL_Renderer *rend) {
    pushText("MENU", HEIGHT / 6, 0, 0);
    pushText("start", 4 * HEIGHT / 12, 0, 0);
    pushText("highscores", 5 * HEIGHT / 12, 0, 0);
    pushText("replay", 6 * HEIGHT / 12, 0, 0);
    pushText("save", 7 * HEIGHT / 12, 0, 0);
    pushText("settings", 8 * HEIGHT / 12, 0, 0);
    pushText("how to play", 9 * HEIGHT / 12, 0, 0);
    pushText("credits", 10 * HEIGHT / 12, 0, 0);
    c.make1();
    image = IMG_LoadTexture(rend, "media/menu.png");
}
void Screen::makeSettings(SDL_Renderer *rend) {
    pushText("SETTINGS", HEIGHT / 6, 0, 0);
    pushText("window size: FULLSCREEN", 4 * HEIGHT / 10, 0, 0);
    pushText("window size: W. FULLSCREEN", 4 * HEIGHT / 10, 0, 0);
    pushText("window size: 640x480", 4 * HEIGHT / 10, 0, 0);
    pushText("VSYNC: ON", 5 * HEIGHT / 10, 0, 0);
    pushText("VSYNC: OFF", 5 * HEIGHT / 10, 0, 0);
    c.make2();
    image = IMG_LoadTexture(rend, "media/menu.png");
}
void Screen::makeUsername(SDL_Renderer *rend) {
    pushText("please enter username:", HEIGHT / 3, 0, 0);
    pushText("", 5 * HEIGHT / 10, 0, 0);
    image = IMG_LoadTexture(rend, "media/menu.png");
}
void Screen::makeHighscore(SDL_Renderer *rend) {
    pushText("HIGHSCORES", HEIGHT / 6, 0, 0);
    image = IMG_LoadTexture(rend, "media/menu.png");
    std::ifstream in("highscores.txt");
    if (!in.is_open())
        return;
    std::string name, scor;
    for (int i = 1; in >> name >> scor && i < 11; i++) {
        pushText(name + "........." + scor, HEIGHT / 3 + i * (50), 0, 0);
    }
    in.close();
}
void Screen::makeHowToPlay(SDL_Renderer *rend) {
    pushText("HOW TO PLAY", HEIGHT / 6, 0, 0);
    pushText("Use keys w, a, s, d, space", 8 * HEIGHT / 20, 0, 0);
    pushText("Killing enemies brings points", 9 * HEIGHT / 20, 0, 0);
    pushText("There are arenas hidden, number of them is shown", 10 * HEIGHT / 20, 0, 0);
    pushText("Inside each arena is a bull", 11 * HEIGHT / 20, 0, 0);
    pushText("Bull gives you one health point", 12 * HEIGHT / 20, 0, 0);
    image = IMG_LoadTexture(rend, "media/menu.png");
}
void Screen::makeCredits(SDL_Renderer *rend) {
    pushText("CREDITS", HEIGHT / 6, 0, 0);
    pushText("Developer: Gasper Nemgar", 4 * HEIGHT / 10, 0, 0);
    pushText("Mentour: Natasa Makarovic", 5 * HEIGHT / 10, 0, 0);
    image = IMG_LoadTexture(rend, "media/menu.png");
}
bool Screen::death(SDL_Event event, Keyb &k, SDL_Renderer *rend) {
    bool run = 1;
    display(rend);
    SDL_RenderPresent(rend);
    SDL_Delay(1000);
    while (run) {
        while (SDL_PollEvent(&event))
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
        if (k.get(SDL_SCANCODE_ESCAPE))
            return 0;
        if (k.get(SDL_SCANCODE_SPACE))
            return 1;
        SDL_Delay(50);
    }
    return 0;
}
int Screen::menu(SDL_Event event, Keyb &k, SDL_Renderer *rend) {
    bool run = 1;
    while (run) {
        while (SDL_PollEvent(&event)) {
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, image, NULL, NULL);
            display(rend);
            c.display(rend);
            SDL_RenderPresent(rend);
        }
        SDL_Delay(100);
        if (k.get(SDL_SCANCODE_ESCAPE)) {
            SDL_Delay(50);
            return -1;
        }
        if (k.get(SDL_SCANCODE_W))
            c.moveUp();
        if (k.get(SDL_SCANCODE_S))
            c.moveDown();
        if (k.get(SDL_SCANCODE_SPACE))
            return c.getCoorPos();
    }
    return 0;
}
int Screen::settings(SDL_Event event, Keyb &k, SDL_Renderer *rend, SDL_Window *wind, Minotaur mino, Screen &menu, Screen &death) {
    bool run = 1;
    int mode;
    while (run) {
        std::cout << WIDTH << " " << HEIGHT << "\n";
        while (SDL_PollEvent(&event)) {
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, image, NULL, NULL);
            renderBlendedBackground(rend);
            display(rend, 0);
            DISPLAY_MODE == 0 ? display(rend, 1) : DISPLAY_MODE == 1 ? display(rend, 2)
                                                                     : display(rend, 3);
            VSYNC ? display(rend, 4) : display(rend, 5);
            c.display(rend);
            SDL_RenderPresent(rend);
        }
        SDL_Delay(50);
        if (k.get(SDL_SCANCODE_ESCAPE)) {
            SDL_Delay(50);
            return 0;
        }
        if (k.get(SDL_SCANCODE_W))
            c.moveUp();
        if (k.get(SDL_SCANCODE_S))
            c.moveDown();
        if (k.get(SDL_SCANCODE_SPACE)) {
            if (c.getCoorPos() == 1) {
                SDL_DisplayMode mode;
                SDL_GetWindowDisplayMode(wind, &mode);
                if (DISPLAY_MODE == 0) {
                    SDL_SetWindowFullscreen(wind, 0);
                    SDL_SetWindowSize(wind, WIDTH, HEIGHT - 80);
                    SDL_SetWindowPosition(wind, 0, 30);
                    SDL_GetWindowSize(wind, &WIDTH, &HEIGHT);
                    updateScreenOnChangeFormat(rend, mino, death, menu, *this);
                } else if (DISPLAY_MODE == 1) {
                    SDL_SetWindowFullscreen(wind, 0);
                    SDL_SetWindowSize(wind, 640, 480);
                    SDL_GetWindowSize(wind, &WIDTH, &HEIGHT);
                    updateScreenOnChangeFormat(rend, mino, death, menu, *this);
                } else {
                    SDL_SetWindowFullscreen(wind, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    SDL_GetWindowSize(wind, &WIDTH, &HEIGHT);
                    updateScreenOnChangeFormat(rend, mino, death, menu, *this);
                    DISPLAY_MODE = 255;
                }
                DISPLAY_MODE++;
            } else if (c.getCoorPos() == 2) {
                VSYNC = !VSYNC;
                SDL_RenderSetVSync(rend, VSYNC);
            }
        }
    }
    return 0;
}
int Screen::username(SDL_Event event, Keyb &k, SDL_Renderer *rend, std::string &s) {
    std::string x;
    char tmp = 0;
    bool run = 1;
    while (run) {
        while (SDL_PollEvent(&event)) {
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, image, NULL, NULL);
            display(rend);
            SDL_RenderPresent(rend);
        }
        tmp = k.getAny();
        if (k.get(SDL_SCANCODE_SLASH))
            tmp = '_';
        if (tmp != 0) {
            x += tmp;
            popText();
            pushText(x, 5 * HEIGHT / 10, 0, 0);
        }
        SDL_Delay(100);
        if (k.get(SDL_SCANCODE_BACKSPACE)) {
            x.pop_back();
            popText();
            pushText(x, 5 * HEIGHT / 10, 0, 0);
        }
        if (k.get(SDL_SCANCODE_ESCAPE)) {
            SDL_Delay(50);
            return -1;
        }
        if (k.get(SDL_SCANCODE_RETURN)) {
            SDL_Delay(50);
            if (x.size() > 30)
                x.resize(30);
            s = x;
            run = 0;
        }
    }
    return 0;
}
int Screen::dis1(SDL_Event event, Keyb &k, SDL_Renderer *rend) {
    bool run = 1;
    while (run) {
        while (SDL_PollEvent(&event)) {
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, image, NULL, NULL);
            display(rend);
            SDL_RenderPresent(rend);
        }
        SDL_Delay(100);
        if (k.get(SDL_SCANCODE_ESCAPE)) {
            SDL_Delay(50);
            run = 0;
        }
    }
    return 0;
}