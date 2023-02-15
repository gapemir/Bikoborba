Minotaur::Minotaur(SDL_Renderer *rend) {
    dir = 0;
    health = 5;
    r = {100, 100, 64, 64};
    h = {10, 10, 200, 40};
    s = {0, 0, 200, 40};
    score = 0;
    texture = IMG_LoadTexture(rend, "media/mino.png");
    heart = IMG_LoadTexture(rend, "media/heart.png");
    bullets.Btexture = IMG_LoadTexture(rend, "media/bullet.png");
    bullets.head = nullptr;
}
void Minotaur::changeDir(int newD) {
    if (dir != newD)
        dirL = dir;
    dir = newD;
}
void Minotaur::changeDirMove(int d, int speed) {
    changeDir(d);
    switch (d) {
    case UPRIGHT:
        r.y -= speed;
    case RIGHT:
        r.x += speed;
        break;
    case UPLEFT:
        r.x -= speed;
    case UP:
        r.y -= speed;
        break;
    case DOWNLEFT:
        r.y += speed;
    case LEFT:
        r.x -= speed;
        break;
    case DOWNRIGHT:
        r.x += speed;
    case DOWN:
        r.y += speed;
        break;
    default:
        break;
    }
}
void Minotaur::display(SDL_Renderer *rend) {
    bullets.chk();
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (dir == RIGHT)
        flip = SDL_FLIP_HORIZONTAL;
    else if (dir == UP || dir == DOWN)
        if (dirL == RIGHT)
            flip = SDL_FLIP_HORIZONTAL;
    SDL_Rect t = SDL_RectMy(r.x, r.y, r.w * SCALE, r.h * SCALE);
    SDL_RenderCopyEx(rend, texture, NULL, &t, NULL, NULL, flip);
    bullets.display(rend);
    SDL_RenderCopy(rend, heart, &s, &h);
    scor.display(rend, score);
}
void Minotaur::shoot() {
    bullets.push(r.x + 20, r.y + 20, dir);
}
void Minotaur::revive(int &menu) {
    menu = 0;
    s.y = 0;
    health = 5;
    score = 0;
}
void Minotaur::move(Keyb b, int speed) {
    bool x = 0;
    if (b.get(SDL_SCANCODE_W) && b.get(SDL_SCANCODE_D)) {
        changeDirMove(UPRIGHT, speed);
        x = 1;
    }
    if (b.get(SDL_SCANCODE_W) && b.get(SDL_SCANCODE_A)) {
        changeDirMove(UPLEFT, speed);
        x = 1;
    }
    if (b.get(SDL_SCANCODE_S) && b.get(SDL_SCANCODE_D)) {
        changeDirMove(DOWNRIGHT, speed);
        x = 1;
    }
    if (b.get(SDL_SCANCODE_S) && b.get(SDL_SCANCODE_A)) {
        changeDirMove(DOWNLEFT, speed);
        x = 1;
    }
    if (x) return;
    if (b.get(SDL_SCANCODE_W)) changeDirMove(UP, speed);
    if (b.get(SDL_SCANCODE_S)) changeDirMove(DOWN, speed);
    if (b.get(SDL_SCANCODE_D)) changeDirMove(RIGHT, speed);
    if (b.get(SDL_SCANCODE_A)) changeDirMove(LEFT, speed);
}
void Minotaur::changeHealth(bool down) {
    if (down) {
        health--;
        s.y += 40;
    } else {
        health++;
        s.y -= 40;
    }
}