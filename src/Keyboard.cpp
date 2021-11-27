#include "Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


/*
    1 | 2 | 3 | C
    4 | 5 | 6 | D
    7 | 8 | 9 | E
    A | 0 | B | F
*/
Keyboard::Keyboard()
{
    keys[0x1] = sf::Keyboard::Key::Num1;
    keys[0x2] = sf::Keyboard::Key::Num2;
    keys[0x3] = sf::Keyboard::Key::Num3;
    keys[0xC] = sf::Keyboard::Key::Num4;

    keys[0x4] = sf::Keyboard::Key::A;
    keys[0x5] = sf::Keyboard::Key::Z;
    keys[0x6] = sf::Keyboard::Key::E;
    keys[0xD] = sf::Keyboard::Key::R;

    keys[0x7] = sf::Keyboard::Key::Q;
    keys[0x8] = sf::Keyboard::Key::S;
    keys[0x9] = sf::Keyboard::Key::D;
    keys[0xE] = sf::Keyboard::Key::F;

    keys[0xA] = sf::Keyboard::Key::W;
    keys[0x0] = sf::Keyboard::Key::X;
    keys[0xB] = sf::Keyboard::Key::C;
    keys[0xF] = sf::Keyboard::Key::V;
}

bool Keyboard::isKeyDown(Byte key)
{
    return sf::Keyboard::isKeyPressed(keys[key]);
}

bool Keyboard::isKeyUp(Byte key)
{
    return !sf::Keyboard::isKeyPressed(keys[key]);
}

Byte Keyboard::waitAnyKey()
{
    while (true)
    {
        for (int i = 0; i < 16; i++)
        {
            if (isKeyDown(i))
                return i;
        }
    }
}
