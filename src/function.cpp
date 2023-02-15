#include <SDL2/SDL.h>
#include <iostream>

#define BSPEED 7
#define MSPEED 5
#define FSPEED 3
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

extern int WIDTH, HEIGHT;

class Bullet {
public:
    int origin; // 0=mino, 1=fighter
    int dir;
    SDL_Rect r;
    Bullet *next;
    Bullet(int, int, int, int);
};
Bullet::Bullet(int xb, int yb, int dire, int orig) {
    r.x = xb;
    r.y = yb;
    dir = dire;
    r.h = 3;
    r.w = 8;
    next = nullptr;
    origin = orig;
}

class BulletList {
    Bullet *head;
    SDL_Texture *texture;

public:
    BulletList(SDL_Renderer *);
    void push(int, int, int, int);
    void display(SDL_Renderer *);
    void update();
    void chk();
    friend class BullfighterList;
};
BulletList::BulletList(SDL_Renderer *rend) {
    head = nullptr;
    SDL_Surface *image;
    image = SDL_LoadBMP("media/bullet.bmp");
    texture = SDL_CreateTextureFromSurface(rend, image);
}
void BulletList::push(int xb, int yb, int dire, int orig) {
    Bullet *newB = new Bullet(xb, yb, dire, orig);
    newB->next = head;
    head = newB;
};
void BulletList::display(SDL_Renderer *rend) {
    int di[] = {270, 90, 0, 180};
    Bullet *tmp = head;
    while (tmp != nullptr) {
        SDL_RenderCopyEx(rend, texture, NULL, &tmp->r, di[tmp->dir], NULL, SDL_FLIP_NONE);
        tmp = tmp->next;
    }
}
void BulletList::update() {
    Bullet *tmp = head;
    while (tmp != nullptr) {
        switch (tmp->dir) {
        case RIGHT:
            tmp->r.x += BSPEED;
            break;
        case UP:
            tmp->r.y -= BSPEED;
            break;
        case LEFT:
            tmp->r.x -= BSPEED;
            break;
        case DOWN:
            tmp->r.y += BSPEED;
            break;
        }
        tmp = tmp->next;
    }
}
void BulletList::chk() {
    Bullet *c = head, *p = nullptr;
    while (c != nullptr) {
        if (c->r.x < 0 || c->r.x > WIDTH || c->r.y < 0 || c->r.y > HEIGHT) {
            p == nullptr ? head = c->next : p->next = c->next;
            delete c;
            p == nullptr ? c = head : c = p;
        }
        p = c;
        if (c == nullptr)
            return;
        c = c->next;
    }
}

class Minotaur {
    int dir, dirL;
    SDL_Texture *texture;

public:
    SDL_Rect r;
    Minotaur(SDL_Renderer *);
    void changeDir(int); //(Z==true) zdej
    int getDir();
    void display(SDL_Renderer *);
    void shoot(BulletList &);
    void move(int, int);
};
Minotaur::Minotaur(SDL_Renderer *rend) {
    dir = 0;
    r.h = 40;
    r.w = 40;
    r.x = 100;
    r.y = 100;
    SDL_Surface *image;
    image = SDL_LoadBMP("media/mino.bmp");
    texture = SDL_CreateTextureFromSurface(rend, image);
}
void Minotaur::changeDir(int newD) {
    if (dir != newD)
        dirL = dir;
    dir = newD;
}
int Minotaur::getDir() {
    return dir;
}
void Minotaur::display(SDL_Renderer *rend) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (dir == RIGHT)
        flip = SDL_FLIP_HORIZONTAL;
    else if (dir == UP || dir == DOWN)
        if (dirL == RIGHT)
            flip = SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(rend, texture, NULL, &r, NULL, NULL, flip);
}
void Minotaur::shoot(BulletList &bullets) {
    bullets.push(r.x + 20, r.y + 20, dir, 0);
}
void Minotaur::move(int dir, int speed) {
    changeDir(dir);
    if (dir == UP) r.y -= speed;
    if (dir == DOWN) r.y += speed;
    if (dir == RIGHT) r.x += speed;
    if (dir == LEFT) r.x -= speed;
}

class Bullfighter {
public:
    SDL_Rect r;
    int dir, dirL;
    Bullfighter *next;
    Bullfighter();
};
Bullfighter::Bullfighter() {
    r.h = 40;
    r.w = 40;
    dir = 0;
}

class BullfighterList {
    Bullfighter *head;
    SDL_Texture *texture;

public:
    BullfighterList(SDL_Renderer *);
    void push(int, int);
    void display(SDL_Renderer *);
    void update(BulletList &);
    void chk(BulletList &);
};
BullfighterList::BullfighterList(SDL_Renderer *rend) {
    head = nullptr;
    SDL_Surface *image;
    image = SDL_LoadBMP("media/bullfighter.bmp");
    texture = SDL_CreateTextureFromSurface(rend, image);
}
void BullfighterList::push(int xb, int yb) {
    Bullfighter *newB = new Bullfighter();
    newB->r.x = xb;
    newB->r.y = yb;
    newB->next = head;
    head = newB;
};
void BullfighterList::display(SDL_Renderer *rend) {
    int di[] = {0, 90, 180, 270};
    Bullfighter *tmp = head;
    while (tmp != nullptr) {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (tmp->dir == 0)
            flip = SDL_FLIP_HORIZONTAL;
        else if (tmp->dir == 1 || tmp->dir == 3)
            if (tmp->dirL == 0)
                flip = SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(rend, texture, NULL, &tmp->r, NULL, NULL, flip);
        tmp = tmp->next;
    }
}
void BullfighterList::update(BulletList &b) {
    Bullfighter *tmp = head;
    while (tmp != nullptr) {
        if (rand() % 20 == 0)
            switch (tmp->dir) {
            case 0:
                tmp->r.x += BSPEED;
                break;
            case 1:
                tmp->r.y -= BSPEED;
                break;
            case 2:
                tmp->r.x -= BSPEED;
                break;
            case 3:
                tmp->r.y += BSPEED;
                break;
            }
        else if (rand() % 50 == 0) {
            tmp->dirL = tmp->dir;
            tmp->dir = rand() % 4;
        } else if (rand() % 10 == 0) {
            b.push(tmp->r.x + 20, tmp->r.y + 20, tmp->dir, 1);
        }
        tmp = tmp->next;
    }
}
void BullfighterList::chk(BulletList &bullets) {
    Bullfighter *tmpf = head, *tmpf_ = nullptr;
    while (tmpf != nullptr) {
        Bullet *tmpb = bullets.head, *tmpb_ = nullptr;
        while (tmpb != nullptr) {
            if (tmpb->r.y + tmpb->r.h > tmpf->r.y && tmpb->r.y < tmpf->r.y + tmpf->r.h)
                if (tmpb->r.x + tmpb->r.w > tmpf->r.x && tmpb->r.x < tmpf->r.x + tmpf->r.w)
                    if (tmpb->origin == 0) { // TODO sprement bullet se spona direkt na bullfighterju
                        tmpf_ == nullptr ? head = tmpf->next : tmpf_->next = tmpf->next;
                        tmpb_ == nullptr ? bullets.head = tmpb->next : tmpb_->next = tmpb->next;
                        delete tmpf;
                        delete tmpb;
                        tmpf_ == nullptr ? tmpf = head : tmpf = tmpf_;
                        tmpb_ == nullptr ? tmpb = bullets.head : tmpb = tmpb_;
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
}
