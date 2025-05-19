#ifndef CONSTS
#define CONSTS
#include "data_structs.h"
#include <cstdint>
#include <string>
#include <fstream>
#include <array>
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
  extern ofstream debug;
}

const int NUM_PAGES = 2;
const int WIDTH = 20;
const int HEIGHT = 7;


const int TYPE_COLORS[] = {15, 208, 172, 129, 136, 243, 0, 10, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 21, 28, 190, 207, 4, 166};


#endif
