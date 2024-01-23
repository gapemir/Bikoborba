Cursor::Cursor() {
}
void Cursor::moveUp() {
    for (int i = 0; i < v1.size(); i++) {
        if (co == v1[i] && i > 0) {
            i--;
            co = v1[i];
            coPos = v2[i];
        }
    }
}
void Cursor::moveDown() {
    for (int i = 0; i < v1.size(); i++) {
        if (co == v1[i] && i < v1.size() - 1) {
            i++;
            co = v1[i];
            coPos = v2[i];
        }
    }
}
void Cursor::display(SDL_Renderer *rend) {
    c.changeRectXY(co.getX(), co.getY());
    c.display(rend, "->");
}
void Cursor::make1() {
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 17, 4 * HEIGHT / 12));
    v2.push_back(1);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 11, 5 * HEIGHT / 12));
    v2.push_back(2);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 15, 6 * HEIGHT / 12));
    v2.push_back(3);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 18, 7 * HEIGHT / 12));
    v2.push_back(4);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 13, 8 * HEIGHT / 12));
    v2.push_back(5);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 11, 9 * HEIGHT / 12));
    v2.push_back(6);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 15, 10 * HEIGHT / 12));
    v2.push_back(7);

    co = v1[0];
    coPos = v2[0];
}
void Cursor::make2() {
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 5, 4 * HEIGHT / 10));
    v2.push_back(1);
    v1.push_back(Coor(WIDTH / 2 - WIDTH / 9, 5 * HEIGHT / 10));
    v2.push_back(2);

    co = v1[0];
    coPos = v2[0];
}
void Cursor::clear() {
    v1.clear();
    v2.clear();
}