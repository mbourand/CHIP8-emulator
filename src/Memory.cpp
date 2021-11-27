#include "Globals.h"
#include "Memory.hpp"

Memory::Memory() {
	memory.fill(0);
}

Byte& Memory::operator[](Word address) {
	return memory[address];
}

const Byte& Memory::operator[](Word address) const {
	return memory[address];
}
