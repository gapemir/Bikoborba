#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#define BSPEED 7
#define MSPEED 4
#define FSPEED 2
#define SCALE 1
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define UPRIGHT 4
#define UPLEFT 5
#define DOWNRIGHT 6
#define DOWNLEFT 7

extern int WIDTH, HEIGHT;

class Bullet {
    int dir;

public:
    SDL_Rect r;
    Bullet *next;
    int getDir() { return dir; }
    Bullet(int, int, int);
};
class BulletList {
    Bullet *head;
    SDL_Texture *Btexture;

public:
    BulletList() {}
    BulletList(SDL_Renderer *);
    void push(int, int, int);
    void display(SDL_Renderer *);
    void update();
    void chk();
    friend class BullfighterList;
    friend class Minotaur;
};

class Text {
    int dw;
    TTF_Font *font;
    SDL_Color White;
    SDL_Rect score;
    void disp1(SDL_Renderer *, SDL_Surface *);
    void changeScoreW(int);
    int NumDigits(int x);

public:
    Text();
    Text(int, int, int, int);
    void display(SDL_Renderer *, std::string);
    void display(SDL_Renderer *, int);
    void changeScoreXY(int, int);
};

class Minotaur {
    BulletList bullets;
    int dir, dirL, health, score;
    SDL_Texture *texture, *heart;
    Text scor;
    SDL_Rect r, h, s;

public:
    Minotaur(SDL_Renderer *);
    void display(SDL_Renderer *);
    void shoot();
    // void chk(BullfighterList);
    void revive(int &);
    void move(Keyb, int);
    void changeHealth(bool);
    void changeDir(int);          //(Z==true) zdej
    void changeDirMove(int, int); //(Z==true) zdej
    void addScore(int i) { score += i; }
    void setBulletsHead(Bullet *x) { x = bullets.head; }
    int getX() { return r.x; }
    int getY() { return r.y; }
    int getW() { return r.w; }
    int getH() { return r.h; }
    int getDir() { return dir; }
    int getScore() { return score; }
    int getHealth() { return health; }
    SDL_Rect getR() { return r; }
    BulletList getBullets() { return bullets; }
};

class Bullfighter {
    int dir, dirL, att;
    bool farmer;

public:
    SDL_Rect r;
    Bullfighter *next;
    Bullfighter();
    void setDir(int d) { dir = d; }
    void setDirL(int d) { dirL = d; }
    void setAtt(int a) { att = a; }
    int getDir() { return dir; }
    int getDirL() { return dirL; }
    int getAtt() { return att; }
    int getFarmer() { return farmer; }
};
class BullfighterList {
    BulletList bullets;
    Bullfighter *head;
    SDL_Texture *texture;
    SDL_Texture *farmer[3];
    void disp1(SDL_Renderer *, Bullfighter *, SDL_RendererFlip);

public:
    BullfighterList(SDL_Renderer *);
    void push(int, int);
    void display(SDL_Renderer *);
    void attack(Bullfighter *);
    void update();
    void chk(Minotaur &);
    void setBulletsHead(Bullet *x) { x = bullets.head; }
    BulletList getBullets() { return bullets; }
};

class Arena {
public:
    SDL_Rect r;
    Arena *next;
    Arena();
};
class ArenaList {
    Arena *head;
    SDL_Texture *texture;

public:
    ArenaList(SDL_Renderer *);
    void push(int xb, int yb);
    void display(SDL_Renderer *);
    void chk(Minotaur);
};
class Screen {
    SDL_Renderer *rend;
    std::vector<Text> t;
    std::vector<std::string> s;
    SDL_Rect r = {0, 0, WIDTH, HEIGHT};
    bool dis = 0;

public:
    Screen(SDL_Renderer *);
    void display(SDL_Renderer *);
    void pushText(std::string, int, int, int, int);
    bool getDis() { return dis; }
    void setDis(bool b) { dis = b; }
    void makeM(Minotaur);
};

#include "My.cpp"

#include "Minotaur.cpp"
#include "Screen.cpp"
#include "Text.cpp"
#include "enemy.cpp"
