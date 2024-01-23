Replay::Replay() {
    b.clear();
}
void Replay::clear() {
    std::ofstream out("replay.bin", std::ios::binary);
    out.close();
    b.clear();
}
void Replay::s(Keyb &k) {
    uint8_t t = 0x00;
    t |= (k.get(SDL_SCANCODE_W) << 0);
    t |= (k.get(SDL_SCANCODE_A) << 1);
    t |= (k.get(SDL_SCANCODE_S) << 2);
    t |= (k.get(SDL_SCANCODE_D) << 3);
    t |= (k.get(SDL_SCANCODE_SPACE) << 4);
    b.push_back(t);
    if (b.size() > 1024)
        save();
}
void Replay::save() {
    std::ofstream out("replay.bin", std::ios::binary | std::ios::app);
    for (int i = 0; i < b.size(); i++)
        out.write((char *)&b[i], sizeof(uint8_t));
    out.close();
    b.clear();
    b.shrink_to_fit();
}
int Replay::play(SDL_Event event, Keyb &k, SDL_Renderer *rend) {
    std::ifstream in("replay.bin", std::ios::binary);
    if (!in.is_open())
        return 0;
    uint8_t tmp;
    SDL_Texture *floor = IMG_LoadTexture(rend, "media/floor.png");
    Minotaur mino(rend);
    bool run = 1;
    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now(), stop = start;
    std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    double dt = 0;

    while (in.read((char *)&tmp, sizeof(tmp)) && run) {
        dt = microseconds.count() * 0.000001;
        /*SDL_Event e;
    e.type = SDL_KEYDOWN;
    e.key.keysym.scancode = SDL_SCANCODE_W;
    SDL_PushEvent(&e);*/
        while (SDL_PollEvent(&event))
            (event.type == SDL_QUIT) ? quit(run) : k.update(event);
        k.set(SDL_SCANCODE_W, tmp & (1 << 0));
        k.set(SDL_SCANCODE_A, tmp & (1 << 1));
        k.set(SDL_SCANCODE_S, tmp & (1 << 2));
        k.set(SDL_SCANCODE_D, tmp & (1 << 3));
        k.set(SDL_SCANCODE_SPACE, tmp & (1 << 4));

        if (k.get(SDL_SCANCODE_ESCAPE)) run = false;
        if (k.get(SDL_SCANCODE_W) || k.get(SDL_SCANCODE_S) || k.get(SDL_SCANCODE_D) || k.get(SDL_SCANCODE_A)) mino.move(k, MSPEED, dt);
        if (k.get(SDL_SCANCODE_SPACE)) mino.shoot(dt);
        mino.getBullets().update(dt);

        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, floor, NULL, NULL);
        mino.display(rend);

        SDL_RenderPresent(rend);

        start = std::chrono::high_resolution_clock::now();
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(start - stop);
        stop = start;
    }
    in.close();
    k.clear();
    return 0;
}
