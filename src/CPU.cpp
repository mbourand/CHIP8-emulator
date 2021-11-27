#include "Globals.h"
#include "CPU.hpp"
#include <SFML/Audio.hpp>

CPU::CPU() : PC(0x200), SP(0)
{
	display = Display(this->memory);
    V.fill(0);
    stack.fill(0);
    timersLast = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void CPU::loadRom(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (file.good() && file.is_open())
	{
		std::streampos length = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(length);
		file.read(buffer.data(), length);
		for (std::size_t i = 0; i < buffer.size(); i++)
			this->memory[PROGRAM_START + i] = buffer[i];
		file.close();
	}
	else
		throw std::runtime_error("Could not open file " + filename);
	std::cout << "Memory Dump : " << std::endl;
	for (std::size_t i = 0x200; i < Memory::RAM_SIZE; i++)
	{
		if (i % 16 == 0)
			std::cout << std::endl;
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)memory[i] << " ";
	}
    std::cout << std::endl;
}

Byte CPU::fetchByte()
{
    return memory[PC++];
}

Word CPU::fetchWord()
{
    Word word = fetchByte() << 8;
    word |= fetchByte();
    return word;
}

std::vector<Byte> CPU::readBytes(Word addr, Byte n) const
{
	std::vector<Byte> bytes;
	for (Byte i = 0; i < n; i++)
		bytes.push_back(memory[addr + i]);
	return bytes;
}

void CPU::handleTimers()
{
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	timersDelta += now - timersLast;
	while (timersDelta > TIMER_RATE)
	{
		if (DT > 0)
			DT--;
		if (ST > 0)
        {
            speaker.play();
			ST--;
        }
        else
            speaker.stop();
		timersDelta -= TIMER_RATE;
	}
	timersLast = now;
}

void CPU::next()
{
	// Handle timers
	handleTimers();

    // Fetch opcode
    Word opcode = fetchWord();
    if (!opcode)
        return;
    switch (opcode) {
        case 0x00E0:
            // CLS
            // Clear the display.
            display.clear();
            break;
        case 0x00EE:
            // RET
            // Return from a subroutine.
            PC = stack[SP--];
            break;
    }
    if (opcode >= 0x1000 && opcode < 0x2000) {
        // 1NNN: JP addr
        // Jump to address NNN.
        PC = opcode & 0x0FFF;
    }
    else if (opcode >= 0x2000 && opcode < 0x3000) {
        // 2NNN: CALL addr
        // Call subroutine at NNN.
        stack[++SP] = PC;
        PC = opcode & 0x0FFF;
    }
    else if (opcode >= 0x3000 && opcode < 0x4000) {
        // 3XKK: SE Vx, KK
        // Skip next instruction if Vx = KK.
        if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            PC += 2;
    }
    else if (opcode >= 0x4000 && opcode < 0x5000) {
        // 4XKK: SNE Vx, KK
        // Skip next instruction if Vx != KK.
        if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            PC += 2;
    }
    else if (opcode >= 0x5000 && opcode < 0x6000 && (opcode & 0xF) == 0)
    {
        // 5XY0: SE Vx, Vy
        // Skip next instruction if Vx = Vy.
        if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            PC += 2;
    }
    else if (opcode >= 0x6000 && opcode < 0x7000)
    {
        // 6XKK: LD Vx, KK
        // Set Vx = kk.
        V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
    }
    else if (opcode >= 0x7000 && opcode < 0x8000)
    {
        // 7xkk - ADD Vx, byte
        // Set Vx = Vx + kk.
        V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
    }
    else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 0)
    {
        // 8xy0 - LD Vx, Vy
        // Set Vx = Vy.
        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
    }
    else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 1)
    {
        // 8xy1 - OR Vx, Vy
        // Set Vx = Vx OR Vy.
        V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
    }
    else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 2)
    {
        // 8xy2 - AND Vx, Vy
        // Set Vx = Vx AND Vy.
        V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
    }
    else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 3)
    {
        // 8xy3 - XOR Vx, Vy
        // Set Vx = Vx XOR Vy.
        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
    }
    else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 4)
    {
        // 8xy4 - ADD Vx, Vy
        // Set Vx = Vx + Vy, set VF = carry.
        V[CPU::F] = (V[(opcode & 0x00F0) >> 4] > BYTE_MAX - V[(opcode & 0x0F00) >> 8]);
        V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
    }
    else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 5)
    {
        // 8xy5 - SUB Vx, Vy
        // Set Vx = Vx - Vy, set VF = NOT borrow.

		V[CPU::F] = (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]);
		V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
    }
	else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 6)
	{
		// 8xy6 - SHR Vx {, Vy}
		// Set Vx = Vx SHR 1.
		V[CPU::F] = (V[(opcode & 0x0F00) >> 8] & 1);
		V[(opcode & 0x0F00) >> 8] >>= 1;
	}
	else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 7)
	{
		// 8xy7 - SUBN Vx, Vy
		// Set Vx = Vy - Vx, set VF = NOT borrow.
		V[CPU::F] = (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]);
		V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
	}
	else if (opcode >= 0x8000 && opcode < 0x9000 && (opcode & 0xF) == 0xE)
	{
		// 8xyE - SHL Vx {, Vy}
		// Set Vx = Vx SHL 1.
		V[CPU::F] = ((V[(opcode & 0x0F00) >> 8] & 0b10000000) != 0);
		V[(opcode & 0x0F00) >> 8] <<= 1;
	}
	else if (opcode >= 0x9000 && opcode < 0xA000 && (opcode & 0xF) == 0)
	{
		// 9xy0 - SNE Vx, Vy
		// Skip next instruction if Vx != Vy.
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			PC += 2;
	}
	else if (opcode >= 0xA000 && opcode < 0xB000)
	{
		// Annn - LD I, addr
		// Set I = nnn.
		I = (opcode & 0x0FFF);
	}
	else if (opcode >= 0xB000 && opcode < 0xC000)
	{
		// Bnnn - JP V0, addr
		// Jump to location nnn + V0.
		PC = (opcode & 0x0FFF) + V[0];
	}
	else if (opcode >= 0xC000 && opcode < 0xD000)
	{
		// Cxkk - RND Vx, byte
		// Set Vx = random byte AND kk.
		V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
	}
	else if (opcode >= 0xD000 && opcode < 0xE000)
	{
		// Dxyn - DRW Vx, Vy, nibble
		// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

		auto bytes = readBytes(I, (opcode & 0xF));
		V[CPU::F] = display.render(bytes, V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4]);
	}
	else if (opcode >= 0xE000 && opcode < 0xF000 && (opcode & 0x00FF) == 0x9E)
	{
		// Ex9E - SKP Vx
		// Skip next instruction if key with the value of Vx is pressed.
		if (keyboard.isKeyDown(V[(opcode & 0x0F00) >> 8]))
			PC += 2;
	}
	else if (opcode >= 0xE000 && opcode < 0xF000 && (opcode & 0x00FF) == 0xA1)
	{
		// ExA1 - SKNP Vx
		// Skip next instruction if key with the value of Vx is not pressed.
		if (keyboard.isKeyUp(V[(opcode & 0x0F00) >> 8]))
			PC += 2;
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x07)
	{
		// Fx07 - LD Vx, DT
		// Set Vx = delay timer value.
		V[(opcode & 0x0F00) >> 8] = DT;
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x0A)
	{
		// Fx0A - LD Vx, K
		// Wait for a key press, store the value of the key in Vx.
		V[(opcode & 0x0F00) >> 8] = keyboard.waitAnyKey();
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x15)
	{
		// Fx15 - LD DT, Vx
		// Set delay timer = Vx.
		DT = V[(opcode & 0x0F00) >> 8];
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x18)
	{
		// Fx18 - LD ST, Vx
		// Set sound timer = Vx.
		ST = V[(opcode & 0x0F00) >> 8];
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x1E)
	{
		// Fx1E - ADD I, Vx
		// Set I = I + Vx.
		I += V[(opcode & 0x0F00) >> 8];
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x29)
	{
		// Fx29 - LD F, Vx
		// Set I = location of sprite for digit Vx.
		I = display.locateDigitSprite(V[(opcode & 0x0F00) >> 8]);
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x33)
	{
		// Fx33 - LD B, Vx
		// Store BCD representation of Vx in memory locations I, I+1, and I+2.
		memory[I    ] = V[(opcode & 0x0F00) >> 8] / 100;
		memory[I + 1] = (V[(opcode & 0x0F00) >> 8] % 100) / 10;
		memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 10);
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x55)
	{
		// Fx55 - LD [I], Vx
		// Store registers V0 through Vx in memory starting at location I.
		for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++)
			memory[I + i] = V[i];
	}
	else if (opcode >= 0xF000 && (opcode & 0x00FF) == 0x65)
	{
		// Fx65 - LD Vx, [I]
		// Read registers V0 through Vx from memory starting at location I.
		for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++)
			V[i] = memory[I + i];
	}
}
