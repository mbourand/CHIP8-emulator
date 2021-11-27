#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "Globals.h"
#include "Memory.hpp"
#include <SFML/Graphics.hpp>

class Display {
    public:
        static constexpr Byte DISPLAY_WIDTH = 64;
        static constexpr Byte DISPLAY_HEIGHT = 32;
	private:
		std::array<Byte, DISPLAY_WIDTH * DISPLAY_HEIGHT> pixels;

	public:
		Display() = default;
		Display(Memory& mem);

		void clear();
        Byte render(std::vector<Byte>& bytes, Byte x, Byte y);
        Word locateDigitSprite(Byte digit);
        void displayOnScreen(sf::RenderWindow& window);
};

#endif
