Bullfighter::Bullfighter() {
    r.h = 64;
    r.w = 64;
    dir = 0;
    att = 0;
    farmer = rand() % 2;
}

BullfighterList::BullfighterList(SDL_Renderer *rend) {
    head = nullptr;
    bullets.head = nullptr;
    bullets.Btexture = IMG_LoadTexture(rend, "media/bullet.png");
    texture = IMG_LoadTexture(rend, "media/bullfighter.png");
    for (int i = 0; i < 3; i++)
        farmer[i] = IMG_LoadTexture(rend, ("media/farmer" + std::to_string(i) + ".png").c_str());
}
void BullfighterList::push(int xb, int yb) {
    Bullfighter *newB = new Bullfighter();
    newB->r.x = xb;
    newB->r.y = yb;
    newB->next = head;
    head = newB;
};
void BullfighterList::display(SDL_Renderer *rend) {
    bullets.display(rend);
    Bullfighter *tmp = head;
    while (tmp != nullptr) {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (tmp->getDir() == 0)
            flip = SDL_FLIP_HORIZONTAL;
        else if (tmp->getDir() == 1 || tmp->getDir() == 3)
            if (tmp->getDirL() == 0)
                flip = SDL_FLIP_HORIZONTAL;
        disp1(rend, tmp, flip);
        tmp = tmp->next;
    }
}
void BullfighterList::disp1(SDL_Renderer *rend, Bullfighter *tmp, SDL_RendererFlip flip) {
    SDL_Rect t = SDL_RectMy(tmp->r.x, tmp->r.y, tmp->r.w * SCALE, tmp->r.h * SCALE);
    if (tmp->getAtt())
        tmp->setAtt(tmp->getAtt() + 1);
    if (tmp->getAtt() > 30)
        tmp->setAtt(0);
    SDL_RenderCopyEx(rend, tmp->getFarmer() ? farmer[tmp->getAtt() / 10] : texture, NULL, &t, NULL, NULL, flip);
}
void BullfighterList::attack(Bullfighter *tmp) {
    if (tmp->getFarmer())
        tmp->setAtt(tmp->getAtt() + 1);
    else
        bullets.push(tmp->r.x + 20, tmp->r.y + 20, tmp->getDir());
}
void BullfighterList::update() {
    Bullfighter *tmp = head;
    while (tmp != nullptr) {
        if (rand() % 20 == 0)
            switch (tmp->getDir()) {
            case 0:
                tmp->r.x += FSPEED;
                break;
            case 1:
                tmp->r.y -= FSPEED;
                break;
            case 2:
                tmp->r.x -= FSPEED;
                break;
            case 3:
                tmp->r.y += FSPEED;
                break;
            }
        else if (rand() % 50 == 0) {
            tmp->setDirL(tmp->getDir());
            tmp->setDir(rand() % 4);
        } else if (rand() % 50 == 0) {
            attack(tmp);
        }
        tmp = tmp->next;
    }
}
void BullfighterList::chk(Minotaur &mino) {
    bullets.update();
    bullets.chk();
    Bullfighter *tmpf = head, *tmpf_ = nullptr;
    while (tmpf != nullptr) {
        Bullet *tmpb = mino.getBullets().head, *tmpb_ = nullptr;
        while (tmpb != nullptr) {
            if (tmpb->r.y + tmpb->r.h > tmpf->r.y && tmpb->r.y < tmpf->r.y + tmpf->r.h)
                if (tmpb->r.x + tmpb->r.w > tmpf->r.x && tmpb->r.x < tmpf->r.x + tmpf->r.w) {
                    tmpf_ == nullptr ? head = tmpf->next : tmpf_->next = tmpf->next;
                    tmpb_ == nullptr ? bullets.head = tmpb->next : tmpb_->next = tmpb->next;
                    delete tmpf;
                    delete tmpb;
                    tmpf_ == nullptr ? tmpf = head : tmpf = tmpf_;
                    tmpb_ == nullptr ? tmpb = bullets.head : tmpb = tmpb_;
                    mino.addScore(1);
                    if (tmpf == nullptr)
                        return;
                    break;
                }
            tmpb_ = tmpb;
            tmpb = tmpb->next;
        }
        tmpf_ = tmpf;
        tmpf = tmpf->next;
    }
    mino.getBullets().update();
    Bullet *tmp = getBullets().head, *tmp_ = nullptr;
    SDL_Rect r = mino.getR();
    while (tmp != nullptr) {
        if (tmp->r.y + tmp->r.h > r.y && tmp->r.y < r.y + r.h)
            if (tmp->r.x + tmp->r.w > r.x && tmp->r.x < r.x + r.w) {
                if (tmp_ == nullptr)
                    mino.setBulletsHead(tmp->next);
                else
                    tmp_->next = tmp->next;
                // tmp_ == nullptr ? mino.setBulletsHead(tmp->next) : tmp_->next = tmp->next;
                delete tmp;
                tmp_ == nullptr ? tmp = mino.getBullets().head : tmp = tmp_;
                mino.changeHealth(1);
            }
        tmp_ = tmp;
        if (tmp == nullptr) return;
        tmp = tmp->next;
    }
}