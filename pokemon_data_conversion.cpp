#include <cctype>
#include <cmath>
#include <cstdlib>
#include <string>
#include <windows.h> 
#include "bitstream.h"
#include "data_structs.h"
#include "constants.h"

using namespace std;
using namespace bitstream;

pokemon_data_structure *extract_pokemon_data(int dexnum) {
  pokemon_data_structure *ret = (pokemon_data_structure *)malloc(sizeof(pokemon_data_structure));
  file_bitstream_reader rom(rom_filename);
  rom.seek(0x383de + 0x1c*(dexnum-1));

  for (int i=0; i<11; i++) {
    if (i==6 || i==0) cout << to_string(rom.peek_byte()) << ',';
    ret->buffer[i] = rom.get_byte(); // dexnum to frontsprite_dimensions
  }
  ret->frontsprite_pointer = rom.get_word();
  ret->backsprite_pointer = rom.get_word();

  for (int i=0; i<4; i++) {
    ret->initial_attacks[i] = rom.get_byte();
  }

  ret->growth_rate = (growth_rate)rom.get_byte();

  for (int i=0; i<50; i++) {
    ret->tm_usage[8*(int)floor(i/8) - (i%8) + 7] = rom.get();
  }

  rom.close();

  return ret;
}
