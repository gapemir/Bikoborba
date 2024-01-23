Text::Text() {
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    rect = {0, 0, 0, 0};
}
Text::Text(int x, int y, int w, int h) {
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    rect = {x, y, w, h};
}
Text::Text(int y, int w, int h) {
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    rect = {-1, y, w, h};
}
Text::Text(int y, int w, int h, std::string str) {
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    rect = {-1, y, w, h};
    s = str;
}
Text::Text(int x, int y, int w, int h, std::string str) {
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    rect = {x, y, w, h};
    s = str;
}
void Text::disp1(SDL_Renderer *rend, SDL_Surface *image) {
    SDL_Texture *Message = SDL_CreateTextureFromSurface(rend, image);
    SDL_FreeSurface(image);
    SDL_QueryTexture(Message, NULL, NULL, &rect.w, &rect.h);
    //std::cout << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << std::endl;
    if (rect.x == -1)
        rect.x = WIDTH / 2 - rect.w / 2;
    SDL_RenderCopy(rend, Message, NULL, &rect);
    SDL_DestroyTexture(Message);
}
void Text::display(SDL_Renderer *rend) {
    SDL_Surface *image = TTF_RenderText_Blended(font, s.c_str(), White);
    disp1(rend, image);
}
void Text::display(SDL_Renderer *rend, std::string s) {
    SDL_Surface *image = TTF_RenderText_Blended(font, s.c_str(), White);
    disp1(rend, image);
}
void Text::display(SDL_Renderer *rend, int i) {
    s = std::to_string(i);
    display(rend);
}
void Text::changeRectXY(int a, int b) {
    rect.x = a;
    rect.y = b;
}
/*void Text::changeScoreW(int a) {
    score.w = dw * a;
}
int Text::NumDigits(int x) {
    x = abs(x);
    return (x < 10 ? 1 : (x < 100 ? 2 : (x < 1000 ? 3 : (x < 10000 ? 4 : (x < 100000 ? 5 : (x < 1000000 ? 6 : (x < 10000000 ? 7 : (x < 100000000 ? 8 : (x < 1000000000 ? 9 : 10)))))))));
}*/