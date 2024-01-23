Minotaur::Minotaur(SDL_Renderer *rend) : scor(10, 50, 0, 0) {
    dir = 0;
    health = 5;
    r = {100, 100, 64, 64};
    h = {10, 10, 200, 40};
    s = {0, 0, 200, 40};
    score = 0;
    hit = 0;
    shot = 0;
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
void Minotaur::changeDirMove(int d, int speed, double dt) {
    if (dt > 0.5)
        dt = 0.016703;
    changeDir(d);
    double x = (di[d] / 180.0) * PI;
    r.x += cos(x) * MSPEED * dt;
    r.y += sin(x) * MSPEED * dt;
    if (r.x < 0)
        r.x = 0;
    else if (r.x > WIDTH - 64)
        r.x = WIDTH - 64;
    if (r.y < 0)
        r.y = 0;
    else if (r.y > HEIGHT - 64)
        r.y = HEIGHT - 64;
}
void Minotaur::display(SDL_Renderer *rend) {
    bullets.chk();
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (dir == 2 || dir == 4 || dir == 5)
        flip = SDL_FLIP_HORIZONTAL;
    else if (dir == 0 || dir == 1)
        if (dirL == 2 || dirL == 4 || dirL == 5)
            flip = SDL_FLIP_HORIZONTAL;
    SDL_Rect t = SDL_RectMy(r.x, r.y, r.w * SCALE, r.h * SCALE);
    // if(hit)
    SDL_RenderCopyEx(rend, texture, NULL, &t, NULL, NULL, flip);
    bullets.display(rend);
    SDL_RenderCopy(rend, heart, &s, &h);
    scor.display(rend, score);
}
void Minotaur::shoot(double dt) {
    if (shot < 0) {
        bullets.push(r.x + 20, r.y + 20, dir);
        shot = 0.15;
        return;
    }
    shot -= dt;
}
void Minotaur::revive() {
    s.y = 0;
    health = 5;
}
void Minotaur::move(Keyb &b, int speed, double dt) {
    bool x = 0;
    if (b.get(SDL_SCANCODE_W) && b.get(SDL_SCANCODE_D)) {
        changeDirMove(4, speed, dt);
        return;
    }
    if (b.get(SDL_SCANCODE_W) && b.get(SDL_SCANCODE_A)) {
        changeDirMove(5, speed, dt);
        return;
    }
    if (b.get(SDL_SCANCODE_S) && b.get(SDL_SCANCODE_D)) {
        changeDirMove(6, speed, dt);
        return;
    }
    if (b.get(SDL_SCANCODE_S) && b.get(SDL_SCANCODE_A)) {
        changeDirMove(7, speed, dt);
        return;
    }
    if (b.get(SDL_SCANCODE_W)) changeDirMove(0, speed, dt);
    if (b.get(SDL_SCANCODE_S)) changeDirMove(1, speed, dt);
    if (b.get(SDL_SCANCODE_D)) changeDirMove(2, speed, dt);
    if (b.get(SDL_SCANCODE_A)) changeDirMove(3, speed, dt);
}
void Minotaur::changeHealth(bool down) {
    if (down) {
        health--;
        s.y += 40;
    } else {
        health++;
        s.y -= 40;
    }
    hit = 1.0;
}
void Minotaur::updateHit(double dt) {
    hit -= dt;
    if (hit < 0)
        hit = 0;
}
void Minotaur::destroyBullets() {
    bullets.destroy();
}