
SDL_Rect SDL_RectMy(int x, int y, int w, int h) {
    SDL_Rect tmp = {x, y, w, h};
    return tmp;
}
void quit(bool &running) {
    running = false;
}
void updateScreenOnChangeFormat(SDL_Renderer *rend, Minotaur mino, Screen &death, Screen &menu, Screen &settings) {
    death.clear();
    death.makeDeath(rend, mino);
    menu.clear();
    menu.makeMenu(rend);
    settings.clear();
    settings.makeSettings(rend);
}
void writeToHighscores(std::string name, int scor) {
    std::ofstream out("highscores.tmp.txt");
    std::ifstream in("highscores.txt");
    bool wr = 1;
    std::string n;
    int s, i = 0;
    if (in.is_open()) {
        for (; in >> n >> s && i < 5; i++) {
            if (wr && s < scor) {
                out << name << " " << scor << "\n";
                wr = 0;
                i++;
            }
            out << n << " " << s << "\n";
        }
        in.close();
        remove("highscores.txt");
    }
    if (wr && !i)
        out << name << " " << scor << "\n";
    out.close();
    rename("highscores.tmp.txt", "highscores.txt");
}
void saveSave(Minotaur &mino, BullfighterList &fighters, ArenaList &arenas, std::string &username, int &level, Stat &barnStat) {
    std::ofstream out("save.bin", std::ios::binary);
    Coor tmp;
    int num = username.size();
    char c[num + 1];
    strcpy(c, username.c_str());
    c[num] = '\n';
    // write username
    out.write((char *)&num, sizeof(num));
    out.write((char *)&c, num * sizeof(char));
    // write score
    num = mino.getScore();
    out.write((char *)&num, sizeof(num));
    // write barn
    num = barnStat.getCount();
    out.write((char *)&num, sizeof(num));
    // write level
    num = level;
    out.write((char *)&num, sizeof(num));
    //  write health
    num = mino.getHealth();
    out.write((char *)&num, sizeof(num));
    // write mino
    tmp.set(mino.getX(), mino.getY());
    out.write((char *)&tmp, sizeof(tmp));
    // write main fighters
    out.write((char *)&fighters.size, sizeof(fighters.size));
    for (int i = 0; i < fighters.size; i++) {
        tmp = *fighters.get(i);
        out.write((char *)&tmp, sizeof(tmp));
    }
    // write arenas
    out.write((char *)&arenas.size, sizeof(arenas.size));
    for (int i = 0; i < arenas.size; i++) {
        Arena *a = arenas.get(i);
        tmp = *a;
        out.write((char *)&tmp, sizeof(tmp));
        // write fighters in arena
        out.write((char *)&a->enemys, sizeof(a->enemys));
        for (int j = 0; j < a->enemys; j++) {
            tmp = *a->fighters.get(j);
            out.write((char *)&tmp, sizeof(tmp));
        }
    }
    out.close();
}
void loadSave(SDL_Renderer *rend, Minotaur &mino, BullfighterList &fighters, ArenaList &arenas, std::string &username, int &level, Stat &barnStat) {
    std::ifstream in("save.bin", std::ios::binary);
    if (!in.is_open())
        return;
    int num, numb;
    Coor tmp;
    // read username
    in.read((char *)&num, sizeof(num));
    char c[num];
    in.read((char *)&c, num * sizeof(char));
    username = c;
    // read score
    in.read((char *)&num, sizeof(num));
    mino.setScore(num);
    // read barn
    in.read((char *)&num, sizeof(num));
    barnStat.setCount(num);
    // read level
    in.read((char *)&num, sizeof(num));
    level = num;
    // read health
    in.read((char *)&num, sizeof(num));
    mino.setHealth(num);
    // read mino
    in.read((char *)&tmp, sizeof(tmp));
    mino.setX(tmp.getX());
    mino.setY(tmp.getY());
    // read main fighters
    in.read((char *)&num, sizeof(num));
    for (int i = 0; i < num; i++) {
        in.read((char *)&tmp, sizeof(tmp));
        fighters.push(tmp.getX(), tmp.getY());
    }
    // read arenas
    in.read((char *)&num, sizeof(num));
    for (int i = 0; i < num; i++) {
        in.read((char *)&tmp, sizeof(tmp));
        arenas.push(tmp.getX(), tmp.getY(), rend);
        Arena *a = arenas.get(i);
        // read fighters in arena
        in.read((char *)&numb, sizeof(numb));
        for (int j = 0; j < numb; j++) {
            in.read((char *)&tmp, sizeof(tmp));
            a->fighters.push(tmp.getX(), tmp.getY());
        }
    }
    in.close();
}