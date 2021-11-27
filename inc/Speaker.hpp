#ifndef SPEAKER_HPP
#define SPEAKER_HPP

#include "Globals.h"
#include <SFML/Audio.hpp>

class Speaker
{
    public:
        static constexpr uint32_t SAMPLES = 44100;
        static constexpr uint32_t SAMPLE_RATE = 44100;
        static constexpr uint32_t AMPLITUDE = 30000;
        static constexpr uint32_t FREQUENCY = 800;

        sf::SoundBuffer buffer;
        sf::Sound sound;

        Speaker();
        void play();
        void stop();
};

#endif
