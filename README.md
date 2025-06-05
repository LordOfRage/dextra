# dextra
An in-line Gen 1 Pokédex where all information is extracted from a ROM you supply yourself.
Limited functionality as of now.

## Capabilities

As of the last commit, this project can:

- Show the name and dex number of the pokemon
- Display the frontsprite for the pokemon
- Show typing
- Show TM/HM capabilities
- Show level up moveset
- Show partial evolution data
- Show dex entry, height and weight information
- Show base stats

## How to build

Simply run the command:
`g++ main.cpp -o main.exe bitstream.cpp draw.cpp pokechar.cpp constants.cpp`

## How to use

The ROM file name must be called `pkred_rom.gb`. After renaming the ROM, run the command `<exe-name> <pokemon-name>`.

Some odd Pokémon names are listed here:
Farfetch'd is `farfetch'd`
Mr Mime is `mr.mime` (no space)
Nidoran (male) is `nidoranm`
Nidoran (female) is `nidoranf`

## FAQ (and other complaints)

### Strange characters with strange colouring artefacts appear when I run the command.
This is likely due to the fact that your terminal is not printing the Unicode characters as Unicode characters. To fix this, simply change the character page for your terminal. On Windows CMD, this would be:
`chcp 65001`
If you do not use Windows, please search for your solution online and add an issue on GitHub so I can update the README.

### How to see (insert feature here)?
This project is a work in progress. If you have any suggestions, add an issue.

### Your code sucks.
I know.
