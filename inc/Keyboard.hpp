#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "Globals.h"
#include <SFML/Graphics.hpp>

class Keyboard {
    public:
        std::map<Byte, sf::Keyboard::Key> keys;
        
        Keyboard();
        bool isKeyDown(Byte key);
        bool isKeyUp(Byte key);
        Byte waitAnyKey();
};

#endif
