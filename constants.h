#ifndef CONSTS
#define CONSTS
#include "data_structs.h"
#include <cstdint>
#include <string>
using namespace std;

namespace pokemon_constants {
  extern uint8_t dexnum;
  extern uint8_t id;

  extern pokemon_data_structure *data_struct;
}

namespace file_constants {
  const string rom_filename = "pkred_rom.gb";
  const int typename_pointer = 0x27dae;
  const int pokename_pointer = 0x1c21e;
  const int pokemon_data_struct_pointer = 0x383de;
  const int pokemon_dex_order_pointer = 0x41024;
}

#endif
