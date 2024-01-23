Stat::Stat(SDL_Renderer *rend, const char *a, int y) : t(y, 0, 0) {
    r = {0, 0, 57, 57};
    texture = IMG_LoadTexture(rend, a);
    count = 0;
}
Stat::Stat(SDL_Renderer *rend, const char *a, int x, int y) : t(x, y, 0, 0) {
    r = {0, 0, 57, 57};
    texture = IMG_LoadTexture(rend, a);
    count = 0;
}
void Stat::display(SDL_Renderer *rend) {
    t.display(rend, count);
    r = t.getRect();
    r.w = r.h;
    r.x = r.x - 77;
    SDL_RenderCopy(rend, texture, NULL, &r);
}