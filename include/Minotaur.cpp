Minotaur::Minotaur(SDL_Renderer *rend) {
    dir = 0;
    health = 5;
    r = {100, 100, 64, 64};
    h = {10, 10, 200, 40};
    s = {0, 0, 200, 40};
    texture = IMG_LoadTexture(rend, "media/mino.png");
    heart = IMG_LoadTexture(rend, "media/heart.png");
    score = 0;
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
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (dir == RIGHT)
        flip = SDL_FLIP_HORIZONTAL;
    else if (dir == UP || dir == DOWN)
        if (dirL == RIGHT)
            flip = SDL_FLIP_HORIZONTAL;
    SDL_Rect t = SDL_RectMy(r.x, r.y, r.w * SCALE, r.h * SCALE);
    SDL_RenderCopyEx(rend, texture, NULL, &t, NULL, NULL, flip);
    SDL_RenderCopy(rend, heart, &s, &h);
    scor.display(rend, score);
}
void Minotaur::shoot(BulletList &bullets) {
    bullets.push(r.x + 20, r.y + 20, dir, 0);
}
void Minotaur::revive(int &menu) {
    menu = 0;
    s.y = 0;
    health = 5;
    score = 0;
}
void Minotaur::move(Keyb b, int speed) {
    if (b.get(SDL_SCANCODE_W) && b.get(SDL_SCANCODE_D)) changeDirMove(UPRIGHT, speed);
    if (b.get(SDL_SCANCODE_W) && b.get(SDL_SCANCODE_A)) changeDirMove(UPLEFT, speed);
    if (b.get(SDL_SCANCODE_S) && b.get(SDL_SCANCODE_D)) changeDirMove(DOWNRIGHT, speed);
    if (b.get(SDL_SCANCODE_S) && b.get(SDL_SCANCODE_A)) changeDirMove(DOWNLEFT, speed);
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
void Minotaur::chk(BulletList &bullets) {
    Bullet *tmp = bullets.head, *tmp_ = nullptr;
    while (tmp != nullptr) {
        if (tmp->r.y + tmp->r.h > r.y && tmp->r.y < r.y + r.h)
            if (tmp->r.x + tmp->r.w > r.x && tmp->r.x < r.x + r.w)
                if (tmp->getOrigin() == 1) { // TODO sprement bullet se spona direkt na njem
                    tmp_ == nullptr ? bullets.head = tmp->next : tmp_->next = tmp->next;
                    delete tmp;
                    tmp_ == nullptr ? tmp = bullets.head : tmp = tmp_;
                    changeHealth(1);
                }
        tmp_ = tmp;
        if (tmp == nullptr) return;
        tmp = tmp->next;
    }
}
