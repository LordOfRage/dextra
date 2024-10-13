#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <windows.h> 
#include "bitstream.h"
#include "data_structs.h"
#include "constants.h"

using namespace std;
using namespace bitstream;
using namespace file_constants;

pokemon_data_structure *extract_pokemon_data(int dexnum) {
  pokemon_data_structure *ret = (pokemon_data_structure *)malloc(sizeof(pokemon_data_structure));
  file_bitstream_reader rom(rom_filename);
  rom.seek(pokemon_data_struct_pointer + 0x1c*(dexnum-1));

  for (int i=0; i<11; i++) {
    ret->buffer[i] = rom.get_byte(); // dexnum to frontsprite_dimensions
  }
  ret->frontsprite_pointer = rom.get_word();
  ret->backsprite_pointer = rom.get_word();

  for (int i=0; i<4; i++) {
    ret->initial_attacks[i] = rom.get_byte();
  }

  ret->growth_rate = (growth_rate)rom.get_byte();

  for (int i=0; i<48; i++) {
    ret->tm_usage[8*(int)floor(i/8) - (i%8) + 7] = rom.get();
  }
  
  uint8_t rest = rom.get_byte();

  ret->tm_usage[48] = rest & 0x01;
  ret->tm_usage[49] = rest & 0x02;
  for (int i=0; i<5; i++) {
    ret->hm_usage[i] = rest & (0x04 << i);
  }

  rom.close();

  return ret;
}
