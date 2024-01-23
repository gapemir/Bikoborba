#include <SDL2/SDL.h>
#include <iostream>

class Keyb {
    uint8_t b[64];

public:
    Keyb();
    void update(SDL_Event);
    int getAny();
    bool get(SDL_Scancode);
    void set(SDL_Scancode, bool);
    void clear();
};

Keyb::Keyb() {
    clear();
}
void Keyb::update(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        b[event.key.keysym.scancode / 8] |= (1 << event.key.keysym.scancode % 8);
        // std::cout << (int)b[5] << std::endl;
    } else if (event.type == SDL_KEYUP) {
        b[event.key.keysym.scancode / 8] &= ~(1 << event.key.keysym.scancode % 8);
        // std::cout << (int)b[event.key.keysym.scancode / 8] << std::endl;
    }
}
bool Keyb::get(SDL_Scancode s) {
    return (b[s / 8] & (1 << (s % 8)));
}
void Keyb::set(SDL_Scancode s, bool m) {
    if (m)
        b[s / 8] |= (1 << s % 8);
    else
        b[s / 8] &= ~(1 << s % 8);
}
void Keyb::clear() {
    for (int i = 0; i < 64; i++) {
        b[i] = 0x00;
    }
}
int Keyb::getAny() {
    for (int i = 0; i < 5; i++)
        if (b[i])
            for (int j = 0; j < 8; j++)
                if (b[i] & (1 << j)) {
                    int x = i * 8 + j;
                    if (x > 29) {
                        if (x == 39)
                            return '0';
                        else
                            return x + 19;
                    }
                    return i * 8 + j + 61;
                }
    return 0;
}