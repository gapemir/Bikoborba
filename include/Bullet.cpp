Bullet::Bullet(int xb, int yb, int dire) {
    r.x = xb;
    r.y = yb;
    dir = dire;
    r.h = 8;
    r.w = 32;
    next = nullptr;
}

BulletList::BulletList(SDL_Renderer *rend) {
    head = nullptr;
    Btexture = IMG_LoadTexture(rend, "media/bullet.png");
}
void BulletList::push(int xb, int yb, int dire) {
    Bullet *newB = new Bullet(xb, yb, dire);
    newB->next = head;
    head = newB;
};
void BulletList::display(SDL_Renderer *rend) {
    Bullet *tmp = head;
    // std::cout << "\ns\n";
    while (tmp != nullptr) {
        // std::cout << tmp->getDir() << " ";
        SDL_Rect t = SDL_RectMy(tmp->getX(), tmp->getY(), tmp->getW() * SCALE / 2, tmp->getH() * SCALE);
        SDL_RenderCopyEx(rend, Btexture, NULL, &t, di[tmp->getDir()], NULL, SDL_FLIP_NONE);
        tmp = tmp->next;
        // std::cout<<"1 ";
    }
}
void BulletList::update(double dt) {
    Bullet *tmp = head;
    while (tmp != nullptr) {
        switch (tmp->getDir()) {
        case 4:
            tmp->updateY(-BSPEED * dt);
        case 2:
            tmp->updateX(BSPEED * dt);
            break;
        case 5:
            tmp->updateX(-BSPEED * dt);
        case 0:
            tmp->updateY(-BSPEED * dt);
            break;
        case 7:
            tmp->updateY(BSPEED * dt);
        case 3:
            tmp->updateX(-BSPEED * dt);
            break;
        case 6:
            tmp->updateX(BSPEED * dt);
        case 1:
            tmp->updateY(BSPEED * dt);
        }
        tmp = tmp->next;
    }
}
void BulletList::chk() {
    Bullet *c = head, *p = nullptr;
    while (c != nullptr) {
        if (c->getX() < 0 || c->getX() > WIDTH || c->getY() < 0 || c->getY() > HEIGHT) {
            p == nullptr ? (c->next == nullptr ? head = nullptr : head = c->next) : p->next = c->next;
            delete c;
            p == nullptr ? c = head : c = p;
        }
        p = c;
        if (c == nullptr)
            return;
        c = c->next;
    }
}
void BulletList::destroy() {
    Bullet *tmp = head, *tmp_ = nullptr;
    while (tmp != nullptr) {
        tmp_ = tmp;
        tmp = tmp->next;
        delete tmp_;
    }
    head = nullptr;
}