SDL_Rect SDL_RectMy(int x, int y, int w, int h) {
    SDL_Rect tmp = {x, y, w, h};
    return tmp;
}
void quit(bool &running) {
    running = false;
}

Bullet::Bullet(int xb, int yb, int dire, int orig) {
    r.x = xb;
    r.y = yb;
    dir = dire;
    r.h = 8;
    r.w = 32;
    next = nullptr;
    origin = orig;
}

BulletList::BulletList(SDL_Renderer *rend) {
    head = nullptr;
    texture = IMG_LoadTexture(rend, "media/bullet.png");
}
void BulletList::push(int xb, int yb, int dire, int orig) {
    Bullet *newB = new Bullet(xb, yb, dire, orig);
    newB->next = head;
    head = newB;
};
void BulletList::display(SDL_Renderer *rend) {
    int di[] = {270, 90, 0, 180, 315, 225, 45, 135};
    Bullet *tmp = head;
    while (tmp != nullptr) {
        SDL_Rect t = SDL_RectMy(tmp->r.x, tmp->r.y, tmp->r.w * SCALE / 2, tmp->r.h * SCALE);
        SDL_RenderCopyEx(rend, texture, NULL, &t, di[tmp->getDir()], NULL, SDL_FLIP_NONE);
        tmp = tmp->next;
    }
}
void BulletList::update() {
    Bullet *tmp = head;
    while (tmp != nullptr) {
        switch (tmp->getDir()) {
        case UPRIGHT:
            tmp->r.y -= BSPEED;
        case RIGHT:
            tmp->r.x += BSPEED;
            break;
        case UPLEFT:
            tmp->r.x -= BSPEED;
        case UP:
            tmp->r.y -= BSPEED;
            break;
        case DOWNLEFT:
            tmp->r.y += BSPEED;
        case LEFT:
            tmp->r.x -= BSPEED;
            break;
        case DOWNRIGHT:
            tmp->r.x += BSPEED;
        case DOWN:
            tmp->r.y += BSPEED;
        }
        tmp = tmp->next;
    }
}
void BulletList::chk() {
    Bullet *c = head, *p = nullptr;
    while (c != nullptr) {
        if (c->r.x < 0 || c->r.x > WIDTH || c->r.y < 0 || c->r.y > HEIGHT) {
            p == nullptr ? head = c->next : p->next = c->next;
            delete c;
            p == nullptr ? c = head : c = p;
        }
        p = c;
        if (c == nullptr)
            return;
        c = c->next;
    }
}

Arena::Arena() {
    r.h = 128;
    r.w = 128;
    r.x = 0;
    r.y = 0;
}
ArenaList::ArenaList(SDL_Renderer *rend) {
    head = nullptr;
    texture = IMG_LoadTexture(rend, "media/arena.png");
}
void ArenaList::push(int xb, int yb) {
    Arena *newB = new Arena();
    newB->r.x = xb;
    newB->r.y = yb;
    newB->next = head;
    head = newB;
}
void ArenaList::display(SDL_Renderer *rend) {
    Arena *tmp = head;
    while (tmp != nullptr) {
        SDL_Rect t = SDL_RectMy(tmp->r.x, tmp->r.y, tmp->r.w * SCALE, tmp->r.h * SCALE);
        SDL_RenderCopy(rend, texture, NULL, &t);
        tmp = tmp->next;
    }
}
void ArenaList::chk(Minotaur mino) {
    Arena *tmp = head;
    while (tmp != nullptr) {
        if (mino.getY() + mino.getH() / 2 > tmp->r.y && mino.getY() < tmp->r.y + tmp->r.h / 2)
            if (mino.getX() + mino.getW() / 2 > tmp->r.x && mino.getX() < tmp->r.x + tmp->r.w / 2) {
                tmp->r.x = rand() % WIDTH; //!
                tmp->r.y = rand() % HEIGHT;
            }
        tmp = tmp->next;
    }
}
