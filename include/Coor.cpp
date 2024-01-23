Coor::Coor() {
    x = 0;
    y = 0;
}
Coor::Coor(int xx, int yy) {
    x = xx;
    y = yy;
}
void Coor::set(int xx, int yy) {
    x = xx;
    y = yy;
}
bool Coor::operator==(Coor c) {
    if (x == c.x && y == c.y)
        return true;
    return false;
}
bool Coor::operator<(Coor c) {
    if (x < c.x && y < c.y)
        return true;
    return false;
}
Coor &Coor::operator=(const Coor &c) {
    x = c.x;
    y = c.y;
    return *this;
}
Coor &Coor::operator=(const Arena &t) {
    x = t.r.x;
    y = t.r.y;
    return *this;
}
Coor &Coor::operator=(const Bullfighter &t) {
    x = t.r.x;
    y = t.r.y;
    return *this;
}