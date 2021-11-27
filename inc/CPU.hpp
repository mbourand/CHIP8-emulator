#ifndef CPU_HPP
#define CPU_HPP

#include "Globals.h"
#include "Memory.hpp"
#include "Display.hpp"
#include "Keyboard.hpp"

class CPU {
	enum Hex : uint8_t { A = 0xA, B = 0xB, C = 0xC, D = 0xD, E = 0xE, F = 0xF };

	public:
		Memory memory;
		Display display;
        Keyboard keyboard;

		// 16 general purpose 8-bit registers, V0 to VF
		// The VF register should not be used by any program, as it is used as a flag by some instructions.
		std::array<Byte, 16> V;

		// Program counter
		Word PC;

		// Stack Pointer
		Byte SP;

        // Index register
        Word I;

        // Delay timer
        Byte DT;

        // Sound timer
        Byte ST;

        // Timers delta
        Byte timersDelta;
        std::time_t timersLast;

		std::array<Word, 16> stack;

		CPU();
		Byte fetchByte();
		Word fetchWord();
		void next();
        void handleTimers();
        std::vector<Byte> readBytes(Word addr, Byte n) const;
        void loadRom(const std::string& filename);
};

#endif
