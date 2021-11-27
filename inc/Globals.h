#ifndef GLOBALS_H
#define GLOBALS_H

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>

using Byte = std::uint8_t;
using Word = std::uint16_t;

constexpr Byte BYTE_MAX = 255;
constexpr uint32_t TIMER_RATE = (1/60.0f) * 1000; // 60Hz
constexpr Word PROGRAM_START = 0x200;
constexpr uint32_t CPU_FREQUENCY = (1/500.0f) * 1000; // 500hz
constexpr double TAU = 6.28318;

#endif
