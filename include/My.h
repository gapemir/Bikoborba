#include <Keyboard.cpp>
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define BSPEED 400
#define MSPEED 250
#define FSPEED 100
#define SCALE 1
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define UPRIGHT 4
#define UPLEFT 5
#define DOWNRIGHT 6
#define DOWNLEFT 7
#define PI 3.14159

extern int WIDTH, HEIGHT;
extern bool VSYNC;
extern uint8_t DISPLAY_MODE;
const int di[] = {270, 90, 0, 180, 315, 225, 45, 135};
const int levels[5][3] = {{5, 1, 5}, {5, 3, 5}, {10, 6, 6}, {15, 6, 15}, {20, 10, 20}};

class Bullet {
    int dir;
    SDL_Rect r;

public:
    Bullet *next;
    int getDir() { return dir; }
    int getX() { return r.x; }
    int getY() { return r.y; }
    int getW() { return r.w; }
    int getH() { return r.h; }
    void updateX(int x) { r.x += x; }
    void updateY(int y) { r.y += y; }
    void updateW(int w) { r.w += w; }
    void updateH(int h) { r.h += h; }
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
    void update(double);
    void chk();
    void destroy();
    friend class BullfighterList;
    friend class Minotaur;
};

class Text {
    TTF_Font *font;
    SDL_Color White;
    SDL_Rect rect;
    std::string s;
    void disp1(SDL_Renderer *, SDL_Surface *);
    void changeScoreW(int);
    int NumDigits(int x);

public:
    Text();
    Text(int, int, int);
    Text(int, int, int, int);
    Text(int, int, int, std::string);
    Text(int, int, int, int, std::string);
    void display(SDL_Renderer *, std::string);
    void display(SDL_Renderer *);
    void display(SDL_Renderer *, int);
    SDL_Rect getRect() { return rect; }
    void changeRectXY(int, int);
};

class Minotaur {
    BulletList bullets;
    int dir, dirL, health, score;
    SDL_Texture *texture, *heart;
    Text scor;
    SDL_FRect r;
    SDL_Rect h, s;
    double hit, shot;

public:
    Minotaur(SDL_Renderer *);
    void display(SDL_Renderer *);
    void shoot(double);
    // void chk(BullfighterList);
    void revive();
    void move(Keyb &, int, double);
    void changeHealth(bool);
    void changeDir(int);                  //(Z==true) zdej
    void changeDirMove(int, int, double); //(Z==true) zdej
    void addScore(int i) { score += i; }
    void destroyBullets();
    void setBulletsHead(Bullet *x) { bullets.head = x; }
    void setX(int x) { r.x = x; }
    void setY(int y) { r.y = y; }
    void setScore(int x) { score = x; }
    void setHealth(int x) { health = x; }
    void resetScore() { score = 0; }
    int getX() { return r.x; }
    int getY() { return r.y; }
    int getW() { return r.w; }
    int getH() { return r.h; }
    int getDir() { return dir; }
    bool gethit() { return hit; }
    int getScore() { return score; }
    int getHealth() { return health; }
    void updateHit(double dt);
    SDL_FRect getR() { return r; }
    BulletList getBullets() { return bullets; }
};

class Bullfighter {
    int dir, dirL, att;
    bool farmer;
    SDL_FRect r;

public:
    Bullfighter *next;
    Bullfighter();
    Bullfighter(int, int, bool);
    void setDir(int d) { dir = d; }
    void setDirL(int d) { dirL = d; }
    void setAtt(int a) { att = a; }
    int getDir() { return dir; }
    int getDirL() { return dirL; }
    int getAtt() { return att; }
    int getFarmer() { return farmer; }
    int getX() { return r.x; }
    int getY() { return r.y; }
    int getW() { return r.w; }
    int getH() { return r.h; }
    void setX(int x) { r.x = x; }
    void setY(int y) { r.y = y; }
    void setW(int w) { r.w = w; }
    void setH(int h) { r.h = h; }
    void updateX(int x) { r.x += x; }
    void updateY(int y) { r.y += y; }
    void updateW(int w) { r.w += w; }
    void updateH(int h) { r.h += h; }
    void move(double);
    int geta(Minotaur mino);
    friend class Coor;
};
class BullfighterList {
    BulletList bullets;
    Bullfighter *head;
    SDL_Texture *texture;
    SDL_Texture *farmer[3];
    void disp1(SDL_Renderer *, Bullfighter *, SDL_RendererFlip);

public:
    int size;
    BullfighterList(SDL_Renderer *);
    void push(int, int);
    void display(SDL_Renderer *);
    void attack(Bullfighter *);
    void update(Minotaur &, double);
    int chk(Minotaur &, double);
    void setBulletsHead(Bullet *x) { x = bullets.head; }
    void destroy();
    bool empty();
    Bullfighter *get(int);
    BulletList getBullets() { return bullets; }
};

class Bull {
    SDL_Rect r;
    SDL_Texture *texture;

public:
    bool lockedOnMino, upHealth;
    Bull(SDL_Renderer *);
    void chk(Minotaur &);
    void display(SDL_Renderer *);
};

class Arena {
public:
    Bull b;
    int enemys;
    SDL_Rect r;
    Arena *next;
    BullfighterList fighters;
    Arena(SDL_Renderer *);
    Arena(SDL_Renderer *, int);
    void display(SDL_Renderer *, SDL_Texture *);
    bool chk(Minotaur &);
    friend class Coor;
};
class ArenaList {
    Arena *head;
    SDL_Texture *texture, *floor, *bull;

public:
    int size;
    int inArena(SDL_Renderer *, Minotaur &, Keyb &k, Arena &);
    ArenaList(SDL_Renderer *);
    void push(int, int, SDL_Renderer *);
    void push(int, int, int, SDL_Renderer *);
    void display(SDL_Renderer *, Minotaur mino);
    int chk(SDL_Renderer *, Minotaur &, Keyb &, bool &);
    void destroy();
    Arena *get(int);
};
class Coor {
    int x = 0, y = 0;

public:
    Coor();
    Coor(int, int);
    bool operator==(Coor);
    bool operator<(Coor);
    void set(int, int);
    int getX() { return x; }
    int getY() { return y; }
    Coor &operator=(const Coor &);
    Coor &operator=(const Arena &);
    Coor &operator=(const Bullfighter &);
};
class Cursor {
    Text c;
    Coor co;
    int coPos;
    std::vector<Coor> v1;
    std::vector<int> v2;

public:
    Cursor();
    void moveUp();
    void moveDown();
    void display(SDL_Renderer *);
    void make1();
    void make2();
    int getCoorPos() { return coPos; }
    void clear();
};
class Screen {
    std::vector<Text> t;
    SDL_Rect r = {0, 0, WIDTH, HEIGHT};
    Cursor c;
    SDL_Texture *image;
    void renderBlendedBackground(SDL_Renderer *);

public:
    Screen();
    void display(SDL_Renderer *);
    void display(SDL_Renderer *, int);
    void pushText(std::string, int, int, int);
    void pushText(std::string, int, int, int, int);
    void popText();
    void clear();
    void makeDeath(SDL_Renderer *, Minotaur);
    void makeMenu(SDL_Renderer *);
    void makeSettings(SDL_Renderer *);
    void makeUsername(SDL_Renderer *);
    void makeHighscore(SDL_Renderer *);
    void makeCredits(SDL_Renderer *);
    void makeHowToPlay(SDL_Renderer *);
    bool death(SDL_Event, Keyb &, SDL_Renderer *);
    int menu(SDL_Event, Keyb &, SDL_Renderer *);
    int settings(SDL_Event, Keyb &, SDL_Renderer *, SDL_Window *, Minotaur, Screen &, Screen &);
    int username(SDL_Event, Keyb &, SDL_Renderer *, std::string &);
    int dis1(SDL_Event, Keyb &, SDL_Renderer *);
};
class Replay {
    std::vector<uint8_t> b; // bits 0 w 1 a 2 s 3 d 4 space
public:
    Replay();
    void clear();
    void s(Keyb &);
    void save();
    int play(SDL_Event, Keyb &, SDL_Renderer *);
};
class Stat {
    SDL_Rect r;
    SDL_Texture *texture;
    Text t;
    int count;

public:
    Stat(SDL_Renderer *, const char *, int, int);
    Stat(SDL_Renderer *, const char *, int);
    void display(SDL_Renderer *);
    int getCount() { return count; }
    void setCount(int x) { count = x; }
    void updateCount(int x) { count += x; }
};

// Functions
SDL_Rect SDL_RectMy(int x, int y, int w, int h);
void quit(bool &running);
void updateScreenOnChangeFormat(SDL_Renderer *rend, Minotaur mino, Screen &death, Screen &menu, Screen &settings);
void writeToHighscores(std::string name, int scor);
void saveSave(Minotaur &, BullfighterList &, ArenaList &, std::string &, int &, Stat &);
void loadSave(SDL_Renderer *, Minotaur &, BullfighterList &, ArenaList &, std::string &, int &, Stat &);

#include "Arena.cpp"
#include "Bull.cpp"
#include "Bullet.cpp"
#include "Coor.cpp"
#include "Cursor.cpp"
#include "Enemy.cpp"
#include "Functions.cpp"
#include "Minotaur.cpp"
#include "Replay.cpp"
#include "Screen.cpp"
#include "Stat.cpp"
#include "Text.cpp"