#include "Speaker.hpp"
        
Speaker::Speaker()
{
    sf::Int16 raw[SAMPLES];

    double increment = FREQUENCY / static_cast<float>(SAMPLE_RATE);
    double x = 0;
    for (unsigned i = 0; i < SAMPLES; i++) {
        raw[i] = AMPLITUDE * sin(x * TAU);
        x += increment;
    }

    buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE);

    sound.setBuffer(buffer);

    sound.setLoop(true);
}

void Speaker::play()
{
    if (sound.getStatus() == sf::Sound::Stopped)
        sound.play();
}

void Speaker::stop()
{
    if (sound.getStatus() == sf::Sound::Playing)
        sound.stop();
}
