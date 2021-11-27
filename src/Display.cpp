#include "Globals.h"
#include "Display.hpp"
#include <SFML/Graphics.hpp>

Display::Display(Memory& memory)
{
	pixels.fill(0);

	// Load sprites of characters 0 to F in interpreter area of memory
	// 0
	memory[0x000] = 0xF0;
	memory[0x001] = 0x90;
	memory[0x002] = 0x90;
	memory[0x003] = 0x90;
	memory[0x004] = 0xF0;
	// 1
	memory[0x005] = 0x20;
	memory[0x006] = 0x60;
	memory[0x007] = 0x20;
	memory[0x008] = 0x20;
	memory[0x009] = 0x70;
	// 2
	memory[0x00A] = 0xF0;
	memory[0x00B] = 0x10;
	memory[0x00C] = 0xF0;
	memory[0x00D] = 0x80;
	memory[0x00E] = 0xF0;
	// 3
	memory[0x00F] = 0xF0;
	memory[0x010] = 0x10;
	memory[0x011] = 0xF0;
	memory[0x012] = 0x10;
	memory[0x013] = 0xF0;
	// 4
	memory[0x014] = 0x90;
	memory[0x015] = 0x90;
	memory[0x016] = 0xF0;
	memory[0x017] = 0x10;
	memory[0x018] = 0x10;
	// 5
	memory[0x019] = 0xF0;
	memory[0x01A] = 0x80;
	memory[0x01B] = 0xF0;
	memory[0x01C] = 0x10;
	memory[0x01D] = 0xF0;
	// 6
	memory[0x01E] = 0xF0;
	memory[0x01F] = 0x80;
	memory[0x020] = 0xF0;
	memory[0x021] = 0x90;
	memory[0x022] = 0xF0;
	// 7
	memory[0x023] = 0xF0;
	memory[0x024] = 0x10;
	memory[0x025] = 0x20;
	memory[0x026] = 0x40;
	memory[0x027] = 0x40;
	// 8
	memory[0x028] = 0xF0;
	memory[0x029] = 0x90;
	memory[0x02A] = 0xF0;
	memory[0x02B] = 0x90;
	memory[0x02C] = 0xF0;
	// 9
	memory[0x02D] = 0xF0;
	memory[0x02E] = 0x90;
	memory[0x02F] = 0xF0;
	memory[0x030] = 0x10;
	memory[0x031] = 0xF0;
	// A
	memory[0x032] = 0xF0;
	memory[0x033] = 0x90;
	memory[0x034] = 0xF0;
	memory[0x035] = 0x90;
	memory[0x036] = 0x90;
	// B
	memory[0x037] = 0xE0;
	memory[0x038] = 0x90;
	memory[0x039] = 0xE0;
	memory[0x03A] = 0x90;
	memory[0x03B] = 0xE0;
	// C
	memory[0x03C] = 0xF0;
	memory[0x03D] = 0x80;
	memory[0x03E] = 0x80;
	memory[0x03F] = 0x80;
	memory[0x040] = 0xF0;
	// D
	memory[0x041] = 0xE0;
	memory[0x042] = 0x90;
	memory[0x043] = 0x90;
	memory[0x044] = 0x90;
	memory[0x045] = 0xE0;
	// E
	memory[0x046] = 0xF0;
	memory[0x047] = 0x80;
	memory[0x048] = 0xF0;
	memory[0x049] = 0x80;
	memory[0x04A] = 0xF0;
	// F
	memory[0x04B] = 0xF0;
	memory[0x04C] = 0x80;
	memory[0x04D] = 0xF0;
	memory[0x04E] = 0x80;
	memory[0x04F] = 0x80;
}

Byte Display::render(std::vector<Byte>& bytes, Byte x, Byte y)
{
	// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
	// Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
	// If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
	Byte vf = 0;
	for (size_t i = 0; i < bytes.size(); i++)
	{
		size_t drawY = (y + i) % DISPLAY_HEIGHT;
		for (int shift = 0; shift < 8; shift++)
		{
			size_t drawX = (x + shift) % DISPLAY_WIDTH;
			size_t idx = (drawY * DISPLAY_WIDTH) + drawX;
			if (bytes[i] & (1 << (7 - shift)))
			{
				if (pixels[idx])
					vf = 1;
				pixels[idx] ^= 1;
			}
		}
	}
	return vf;
}

void Display::clear()
{
	pixels.fill(0);
}

Word Display::locateDigitSprite(Byte digit)
{
	return digit * 5;
}

void Display::displayOnScreen(sf::RenderWindow& window)
{
	sf::RectangleShape pixel;
	pixel.setSize(sf::Vector2f(window.getSize().x / DISPLAY_WIDTH, window.getSize().y / DISPLAY_HEIGHT));
	pixel.setFillColor(sf::Color::White);

	for (Byte y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (Byte x = 0; x < DISPLAY_WIDTH; x++)
		{
			if (pixels[y * DISPLAY_WIDTH + x])
			{
				pixel.setPosition(x * pixel.getSize().x, y * pixel.getSize().y);
				window.draw(pixel);
			}
		}
	}
}
