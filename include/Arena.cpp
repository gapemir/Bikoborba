Arena::Arena(SDL_Renderer *rend) : fighters(rend), b(rend) {
    r = {0, 0, 128, 128};

    enemys = rand() % 10 + 2;
    for (int i = 0; i < enemys; i++) {
        fighters.push(rand() % (WIDTH / 2) + WIDTH / 4, rand() % (HEIGHT / 2) + HEIGHT / 4);
    }
}
Arena::Arena(SDL_Renderer *rend, int limit) : fighters(rend), b(rend) {
    r = {0, 0, 128, 128};
    enemys = rand() % limit + 2;
    for (int i = 0; i < enemys; i++) {
        fighters.push(rand() % (WIDTH / 2) + WIDTH / 4, rand() % (HEIGHT / 2) + HEIGHT / 4);
    }
}
void Arena::display(SDL_Renderer *rend, SDL_Texture *texture) {
    SDL_RenderCopy(rend, texture, NULL, &r);
}
bool Arena::chk(Minotaur &mino) {
    if (mino.getY() + mino.getH() / 2 > r.y && mino.getY() < r.y + r.h / 2)
        if (mino.getX() + mino.getW() / 2 > r.x && mino.getX() < r.x + r.w / 2)
            return 1;
    return 0;
}

ArenaList::ArenaList(SDL_Renderer *rend) {
    size = 0;
    head = nullptr;
    texture = IMG_LoadTexture(rend, "media/arena.png");
    floor = IMG_LoadTexture(rend, "media/florarena1.png");
    bull = IMG_LoadTexture(rend, "media/bull.png");
}
void ArenaList::push(int xb, int yb, SDL_Renderer *rend) {
    Arena *newB = new Arena(rend);
    newB->r.x = xb;
    newB->r.y = yb;
    newB->next = head;
    head = newB;
    size++;
}
void ArenaList::push(int xb, int yb, int limit, SDL_Renderer *rend) {
    Arena *newB = new Arena(rend, limit);
    newB->r.x = xb;
    newB->r.y = yb;
    newB->next = head;
    head = newB;
    size++;
}
void ArenaList::display(SDL_Renderer *rend, Minotaur mino) {
    Arena *tmp = head;
    while (tmp != nullptr) {
        SDL_Rect t = SDL_RectMy(tmp->r.x, tmp->r.y, tmp->r.w * SCALE, tmp->r.h * SCALE);
        if (t.x + 200 > mino.getX() && t.x - 200 < mino.getX())
            if (t.y + 200 > mino.getY() && t.y - 200 < mino.getY())
                SDL_RenderCopy(rend, texture, NULL, &t);
        tmp = tmp->next;
    }
}
int ArenaList::chk(SDL_Renderer *rend, Minotaur &mino, Keyb &k, bool &inA) { // 0-normal 1-plus tocka 2-ni ubio useh 3-dead 4-ESC
    Arena *tmp = head, *tmp_ = nullptr;
    while (tmp != nullptr) {
        if (mino.getY() + mino.getH() / 2 > tmp->r.y && mino.getY() < tmp->r.y + tmp->r.h / 2)
            if (mino.getX() + mino.getW() / 2 > tmp->r.x && mino.getX() < tmp->r.x + tmp->r.w / 2) {
                if (!inA) {
                    inA = 1;
                    switch (inArena(rend, mino, k, *tmp)) {
                    case 1:
                        return 2;
                    case 2:
                        return 3;
                    case 3:
                        return 4;
                    default:
                        break;
                    }
                    tmp_ == nullptr ? (tmp->next == nullptr ? head = nullptr : head = tmp->next) : tmp_->next = tmp->next;
                    delete tmp;
                    tmp_ == nullptr ? tmp = head : tmp = tmp_;
                    size--;
                    return 1;
                }
                return 0;
            }
        tmp_ = tmp;
        if (tmp == nullptr)
            return 0;
        tmp = tmp->next;
    }
    if (inA)
        inA = false;
    return 0;
}
void ArenaList::destroy() {
    Arena *tmp = head, *tmp_ = nullptr;
    while (tmp != nullptr) {
        tmp_ = tmp;
        tmp = tmp->next;
        delete tmp_;
    }
    head = nullptr;
    size = 0;
}
int ArenaList::inArena(SDL_Renderer *rend, Minotaur &mino, Keyb &k, Arena &tmp) { // 0-normal 1-ni useh pobio 2-dead, 3-ESC
    bool run = 1, dead = 0, out = 0;
    SDL_Event event;

    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now(), stop = start;
    std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    double dt;

    while (run) {
        dt = microseconds.count() * 0.000001;
        while (SDL_PollEvent(&event))
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
        if (k.get(SDL_SCANCODE_ESCAPE)) run = false;
        if (k.get(SDL_SCANCODE_W) || k.get(SDL_SCANCODE_S) || k.get(SDL_SCANCODE_D) || k.get(SDL_SCANCODE_A)) mino.move(k, MSPEED, dt);
        if (k.get(SDL_SCANCODE_SPACE)) mino.shoot(dt);
        tmp.fighters.update(mino, dt);
        tmp.enemys -= tmp.fighters.chk(mino, dt);
        if (!tmp.chk(mino))
            out = 1;
        else if (out) {
            if (tmp.fighters.empty() && tmp.b.lockedOnMino) {
                return 0;
            } else {
                return 1;
            }
        }
        tmp.b.chk(mino);
        if (!mino.getHealth())
            return 2;
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, floor, NULL, NULL);
        tmp.fighters.display(rend);
        tmp.display(rend, texture);
        tmp.b.display(rend);
        mino.display(rend);
        SDL_RenderPresent(rend);

        start = std::chrono::high_resolution_clock::now();
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(start - stop);
        stop = start;
    }
    return 3;
}
Arena *ArenaList::get(int x) {
    Arena *tmp = head;
    for (int i = 0; i < x && tmp != NULL; i++)
        tmp = tmp->next;
    return tmp;
}