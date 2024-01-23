Bullfighter::Bullfighter() {
    r.h = 64;
    r.w = 64;
    dir = 0;
    att = 0;
    farmer = rand() % 2;
}
Bullfighter::Bullfighter(int x, int y, bool f) {
    r.x = x;
    r.y = y;
    r.h = 64;
    r.w = 64;
    dir = 0;
    att = 0;
    farmer = f;
}

BullfighterList::BullfighterList(SDL_Renderer *rend) {
    head = nullptr;
    bullets.head = nullptr;
    bullets.Btexture = IMG_LoadTexture(rend, "media/bullet.png");
    texture = IMG_LoadTexture(rend, "media/bullfighter.png");
    for (int i = 0; i < 3; i++)
        farmer[i] = IMG_LoadTexture(rend, ("media/farmer" + std::to_string(i) + ".png").c_str());
    size = 0;
}
void BullfighterList::push(int xb, int yb) {
    Bullfighter *newB = new Bullfighter();
    newB->setX(xb);
    newB->setY(yb);
    newB->next = head;
    head = newB;
    size++;
};
void BullfighterList::display(SDL_Renderer *rend) {
    bullets.display(rend);
    Bullfighter *tmp = head;
    while (tmp != nullptr) {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (tmp->getDir() == 2 || tmp->getDir() == 4 || tmp->getDir() == 5)
            flip = SDL_FLIP_HORIZONTAL;
        else if (tmp->getDir() == 0 || tmp->getDir() == 1)
            if (tmp->getDirL() == 2 || tmp->getDirL() == 4 || tmp->getDirL() == 5)
                flip = SDL_FLIP_HORIZONTAL;
        disp1(rend, tmp, flip);
        tmp = tmp->next;
    }
}
void BullfighterList::disp1(SDL_Renderer *rend, Bullfighter *tmp, SDL_RendererFlip flip) {
    SDL_Rect t = SDL_RectMy(tmp->getX(), tmp->getY(), tmp->getW() * SCALE, tmp->getH() * SCALE);
    if (tmp->getAtt())
        tmp->setAtt(tmp->getAtt() + 1);
    if (tmp->getAtt() >= 30)
        tmp->setAtt(0);
    SDL_RenderCopyEx(rend, tmp->getFarmer() ? farmer[tmp->getAtt() / 10] : texture, NULL, &t, NULL, NULL, flip);
}
void BullfighterList::attack(Bullfighter *tmp) {
    if (tmp->getFarmer())
        tmp->setAtt(tmp->getAtt() + 1);
    else
        bullets.push(tmp->getX() + 20, tmp->getY() + 20, tmp->getDir());
}
void Bullfighter::move(double dt) {
    std::cout << dir << "\n";
    if (dt > 0.5)
        dt = 0.016703;
    double x = (di[dir] / 180.0) * PI;
    r.x += cos(x) * FSPEED * dt;
    r.y += sin(x) * FSPEED * dt;
    if (r.x < 0)
        r.x = 0;
    else if (r.x > WIDTH - 64)
        r.x = WIDTH - 64;
    if (r.y < 0)
        r.y = 0;
    else if (r.y > HEIGHT - 64)
        r.y = HEIGHT - 64;
}
int Bullfighter::geta(Minotaur mino) {
    std::cout << getDir() << "\n";
    if (getX() - 32 < mino.getX()) {
        if (getY() - 32 < mino.getY())
            return 6;
        else if (getY() + 32 > mino.getY())
            return 4;
        else
            return 2;
    } else if (getX() + 32 > mino.getX()) {
        if (getY() - 32 < mino.getY())
            return 7;
        else if (getY() + 32 > mino.getY())
            return 5;
        else
            return 3;
    }
    if (getY() + 32 < mino.getY())
        return 1;
    return 0;
}
void BullfighterList::update(Minotaur &mino, double dt) {
    Bullfighter *tmp = head;
    while (tmp != nullptr) {
        /*if (tmp->getX() + 500 > mino.getX() && tmp->getX() - 500 < mino.getX() && tmp->getY() + 500 > mino.getY() && tmp->getY() - 500 < mino.getY()) {
            tmp->setDirL(tmp->getDir());
            tmp->setDir(tmp->geta(mino));
            tmp->move(dt);
        } else */if (rand() % 5 == 0) {
            tmp->move(dt);
            if (tmp->getX() < 0)
                tmp->setX(0);
            else if (tmp->getX() > WIDTH - 64)
                tmp->setX(WIDTH - 64);
            if (tmp->getY() < 0)
                tmp->setY(0);
            else if (tmp->getY() > HEIGHT - 64)
                tmp->setX(HEIGHT - 64);
        } else if (rand() % 50 == 0) {
            tmp->setDirL(tmp->getDir());
            /*if (tmp->getX() + 500 > mino.getX() && tmp->getX() - 500 < mino.getX() && tmp->getY() + 500 > mino.getY() && tmp->getY() - 500 < mino.getY())
                tmp->setDir(tmp->geta(mino));
            else*/
            if (tmp->getX() + 500 > mino.getX() && tmp->getX() - 500 < mino.getX() && tmp->getY() + 500 > mino.getY() && tmp->getY() - 500 < mino.getY()) {
                tmp->setDirL(tmp->getDir());
                tmp->setDir(tmp->geta(mino));
                tmp->move(dt);
            } else
                tmp->setDir(rand() % 8);
        } else if (rand() % 50 == 0) {
            attack(tmp);
        }
        tmp = tmp->next;
    }
}
int BullfighterList::chk(Minotaur &mino, double dt) {
    int count = 0;
    bullets.update(dt);
    bullets.chk();
    Bullfighter *tmpf = head, *tmpf_ = nullptr;
    SDL_FRect r = mino.getR();
    mino.updateHit(dt);
    while (tmpf != nullptr) {
        Bullet *tmpb = mino.getBullets().head, *tmpb_ = nullptr;
        while (tmpb != nullptr) {
            if (tmpb->getY() + tmpb->getH() > tmpf->getY() && tmpb->getY() < tmpf->getY() + tmpf->getH() && tmpb->getX() + tmpb->getW() > tmpf->getX() && tmpb->getX() < tmpf->getX() + tmpf->getW()) {
                tmpf_ == nullptr ? head = tmpf->next : tmpf_->next = tmpf->next;
                if (tmpb_ == nullptr)
                    mino.setBulletsHead(tmpb->next);
                else
                    tmpb_->next = tmpb->next;
                delete tmpf;
                delete tmpb;
                count++;
                tmpf_ == nullptr ? tmpf = head : tmpf = tmpf_;
                tmpb_ == nullptr ? tmpb = mino.getBullets().head : tmpb = tmpb_;
                mino.addScore(1);
                size--;
                if (tmpf == nullptr)
                    return 0;
                break;
            }
            tmpb_ = tmpb;
            tmpb = tmpb->next;
        }
        if (tmpf->getFarmer() && tmpf->getAtt() && tmpf->getY() < r.y + r.h && tmpf->getY() + tmpf->getH() > r.y && tmpf->getX() < r.x + r.w && tmpf->getX() + tmpf->getW() > r.x)
            if (!mino.gethit())
                mino.changeHealth(1);
        tmpf_ = tmpf;
        tmpf = tmpf->next;
    }
    mino.getBullets().update(dt);
    Bullet *tmp = getBullets().head, *tmp_ = nullptr;
    while (tmp != nullptr) {
        if (tmp->getY() + tmp->getH() > r.y && tmp->getY() < r.y + r.h && tmp->getX() + tmp->getW() > r.x && tmp->getX() < r.x + r.w)
            if (!mino.gethit()) {
                mino.changeHealth(1);
                tmp_ == nullptr ? bullets.head = tmp->next : tmp_->next = tmp->next;
                delete tmp;
                tmp_ == nullptr ? tmp = bullets.head : tmp = tmp_;
            }
        tmp_ = tmp;
        if (tmp == nullptr)
            return 0;
        tmp = tmp->next;
    }
    return count;
}
void BullfighterList::destroy() {
    bullets.destroy();
    Bullfighter *tmp = head, *tmp_ = nullptr;
    while (tmp != nullptr) {
        tmp_ = tmp;
        tmp = tmp->next;
        delete tmp_;
    }
    head = nullptr;
    size = 0;
}
bool BullfighterList::empty() {
    if (head == NULL)
        return 1;
    return 0;
}
Bullfighter *BullfighterList::get(int x) {
    Bullfighter *tmp = head;
    for (int i = 0; i < x && tmp != NULL; i++)
        tmp = tmp->next;
    return tmp;
}