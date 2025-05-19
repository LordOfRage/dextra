#include "constants.h"
#include <fstream>

namespace pokemon_constants {
  uint8_t dexnum;
  uint8_t id;

  pokemon_data_structure *data_struct;
}

namespace file_constants {
  ofstream debug("DEBUG.txt");
}
