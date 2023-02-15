#include <SDL2/SDL.h>
#include <iostream>

class Keyb {
    uint8_t b[64];

public:
    Keyb();
    void update(SDL_Event);
    bool get(SDL_Scancode);
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
void Keyb::clear() {
    for (int i = 0; i < 64; i++) {
        b[i] = 0x00;
    }
}