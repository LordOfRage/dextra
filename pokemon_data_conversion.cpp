#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include "bitstream.h"
#include "data_structs.h"
#include "pokechar.h"
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

  ret->growth_rate = (growth_rate_values)rom.get_byte();

  for (int i=0; i<48; i++) {
    ret->tm_usage[8*(int)floor(i/8) - (i%8) + 7] = rom.get();
  }
  
  uint8_t rest = rom.get_byte();

  ret->tm_usage[48] = rest & 0x01;
  ret->tm_usage[49] = rest & 0x02;
  for (int i=0; i<5; i++) {
    ret->hm_usage[i] = rest & (0x04 << i);
  }

  ret->hm_usage[0] = rest & 0x04;
  ret->hm_usage[1] = rest & 0x08;
  ret->hm_usage[2] = rest & 0x10;
  ret->hm_usage[3] = rest & 0x20;
  ret->hm_usage[4] = rest & 0x40;

  rom.close();

  return ret;
}

dexinfo_structure *extract_dex_data(int id) {
  dexinfo_structure *ret = (dexinfo_structure *)malloc(sizeof(dexinfo_structure));

  file_bitstream_reader rom(rom_filename);
  rom.seek(pokedex_info_pointer + 2*(id-1));
  rom.seek(rom.get_word() + 0x3c000);
  
  strcpy(ret->pkmnkind, read_string(rom, 11).c_str());
  ret->height_feet = rom.get_byte();
  ret->height_inches = rom.get_byte();
  ret->weight_tenthpounds = rom.get_word();
  rom.get_byte();
  ret->dexentry_pointer = rom.get_word();

  return ret;
};
