#include "CPU.hpp"

int main(int argc, char **argv)
{
    srand(time(NULL));
    if (argc != 2)
    {
        std::cerr << "Usage: ./emu <path_to_rom>" << std::endl;
        return 1;
    }
	CPU cpu;
    std::cout << "Loading ROM..." << std::endl;
    cpu.loadRom(argv[1]);
    std::cout << "ROM loaded!" << std::endl;

    std::cout << "Starting emulator..." << std::endl;
    sf::RenderWindow window(sf::VideoMode(640, 320), "Chip8 Emulator");
    sf::Clock cpuClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
        }
        if (cpuClock.getElapsedTime().asMilliseconds() > CPU_FREQUENCY)
        {
            cpu.next();
            cpuClock.restart();
        }
        window.clear();
        cpu.display.displayOnScreen(window);
        window.display();
    }
}
