# CHIP8 Emulator
<p align="center">
    <img src="https://www.github.com/mbourand/CHIP8-emulator/raw/master/readme/introduction.gif">
</p>

### Présentation
Ce projet est un interpréteur du langage ![CHIP-8](https://fr.wikipedia.org/wiki/CHIP-8)
Le programme lit une ROM chip8 et interprète son contenu de manière à reproduire le comportement des machines simulées.

### Installation
Choisissez la version correspondant à votre système dans les releases.

### Utilisation
```./emu <chemin_rom>```

### Compilation
```g++ -Iinc -O3 src/* -o emu -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio```
