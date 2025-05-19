#include "data_structs.h"
#include "pokechar.h"
#include "constants.h"
#include "draw.h"
#include "bitstream.h"
#include "pokemon_data_conversion.cpp"
#include <cstdlib>
using namespace std;
using namespace bitstream;

int main(int argc, char **argv) {
  file_bitstream_reader rom("pkred_rom.gb");
  pokemon_data_structure *pokemon = extract_pokemon_data(atoi(argv[1]));
  pokemon_constants::id = get_pokemon_id(atoi(argv[1]));

  draw_pokemon_sprite(rom, pokemon, 1, 1);
  free(pokemon);
  return 0;
}
