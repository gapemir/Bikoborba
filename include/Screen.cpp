Screen::Screen(SDL_Renderer *re) {
    rend = re;
}
void Screen::display(SDL_Renderer *re) {
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 204);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(rend, &r);
    SDL_RenderCopy(rend, NULL, NULL, &r);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    for (int i = 0; i < t.size(); i++) {
        t[i].display(rend, s[i]);
    }
    dis = 1;
}
void Screen::pushText(std::string st, int x, int y, int w, int h) {
    s.push_back(st);
    int z = st.size();
    Text te(WIDTH / 2 - st.size() * h / 4, y, w, h);
    t.push_back(te);
}
void Screen::makeM(Minotaur mino) {
    pushText(std::string("SCORE: ") + std::to_string(mino.getScore()), 3 * WIDTH / 8, HEIGHT / 3, 25, 50);
    pushText("press space to continue...", 3 * WIDTH / 8, 4 * HEIGHT / 5, 15, 30);
}