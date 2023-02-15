Text::Text() {
    dw = 25;
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    score = {10,50,25,50};
}
Text::Text(int x,int y,int w,int h) {
    dw = w;
    font = TTF_OpenFont("media/Prototype.ttf", 48);
    White = {255, 255, 255};
    score = {x, y, w, h};
}
void Text::disp1(SDL_Renderer *rend, SDL_Surface *image) {
    SDL_Texture *Message = SDL_CreateTextureFromSurface(rend, image);
    SDL_FreeSurface(image);
    SDL_QueryTexture(Message,NULL,NULL,&score.w,&score.h);
    SDL_RenderCopy(rend, Message, NULL, &score);
    SDL_DestroyTexture(Message);
}
void Text::display(SDL_Renderer *rend, std::string s) {
    SDL_Surface *image = TTF_RenderText_Blended(font, s.c_str(), White);
    disp1(rend, image);
}
void Text::display(SDL_Renderer *rend, int s) {
    display(rend, std::to_string(s));
}
void Text::changeScoreXY(int a, int b) {
    score.x = a;
    score.y = b;
}
//? DEPRECATED
/*void Text::changeScoreW(int a) {
    score.w = dw * a;
}
int Text::NumDigits(int x) {
    x = abs(x);
    return (x < 10 ? 1 : (x < 100 ? 2 : (x < 1000 ? 3 : (x < 10000 ? 4 : (x < 100000 ? 5 : (x < 1000000 ? 6 : (x < 10000000 ? 7 : (x < 100000000 ? 8 : (x < 1000000000 ? 9 : 10)))))))));
}*/