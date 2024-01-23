Bull::Bull(SDL_Renderer *rend) {
    r = {rand() % (9 * WIDTH / 10), rand() % (9 * HEIGHT / 10), 64, 64};
    texture = IMG_LoadTexture(rend, "media/bull.png");
    lockedOnMino = false;
    upHealth = false;
}
void Bull::chk(Minotaur &mino) {
    if (!lockedOnMino) {
        if (mino.getY() + mino.getH() > r.y && mino.getY() < r.y + r.h && mino.getX() + mino.getW() > r.x && mino.getX() < r.x + r.w)
            lockedOnMino = true;
    } else {
        if (abs((mino.getX() + (mino.getW() / 2)) - (r.x + (r.w / 2))) > 60) {
            if (mino.getX() > r.x)
                r.x++;
            else
                r.x--;
        }
        if (abs((mino.getY() + (mino.getH() / 2)) - (r.y + (r.h / 2))) > 60) {
            if (mino.getY() > r.y)
                r.y++;
            else
                r.y--;
        }
        if (abs((mino.getX() + (mino.getW() / 2)) - (r.x + (r.w / 2))) < 10 && abs((mino.getY() + (mino.getH() / 2)) - (r.y + (r.h / 2))) < 10 && !upHealth) {
            if (mino.getHealth() < 5) {
                mino.changeHealth(0);
                upHealth = true;
            }
        }
    }
}
void Bull::display(SDL_Renderer *rend) {
    SDL_RenderCopy(rend, texture, NULL, &r);
}