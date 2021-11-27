#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Globals.h"

/*
	+---------------+= 0xFFF (4095) End of Chip-8 RAM
	|               |
	|               |
	|               |
	|               |
	|               |
	| 0x200 to 0xFFF|
	|     Chip-8    |
	| Program / Data|
	|     Space     |
	|               |
	|               |
	|               |
	+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
	|               |
	|               |
	|               |
	+---------------+= 0x200 (512) Start of most Chip-8 programs
	| 0x000 to 0x1FF|
	| Reserved for  |
	|  interpreter  |
	+---------------+= 0x000 (0) Start of Chip-8 RAM
*/
class Memory {
	public:
		static constexpr uint32_t RAM_SIZE = 4096;

	private:
		std::array<Byte, RAM_SIZE> memory;

	public:
		Memory();

		Byte& operator[](Word address);
		const Byte& operator[](Word address) const;
};

#endif
