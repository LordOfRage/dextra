#include "bitstream.h"
#include "pokechar.h"
#include "constants.h"
#include <string>
using namespace bitstream;
using namespace file_constants;

string evo_info(uint8_t id) {
  file_bitstream_reader evo_reader(rom_filename);
  string evo_str;

  evo_reader.seek(0x3b05a + 2*id);
  evo_reader.seek(evo_reader.get_word() + (0xd << 14));

  evo_str += get_pokemon_name(id);

  bool more_evolutions = true;

  while (more_evolutions) {
    switch (evo_reader.get_byte()) {
      case 0:
	evo_str += " doesn't evolve";
	more_evolutions = false;
	break;
      case 1:
	evo_str += " evolves at level " + to_string(evo_reader.get_byte());
	evo_str += " into " + get_pokemon_name(evo_reader.get_byte());
	break;
      case 2:
	evo_str += " evolves by using the ";
	  switch (evo_reader.get_byte()) {
	    case 0x0a:
	      evo_str += "Moon Stone";
	      break;
	    case 0x20:
	      evo_str += "Fire Stone";
	      break;
	    case 0x21:
	      evo_str += "Thunder Stone";
	      break;
	    case 0x22:
	      evo_str += "Water Stone";
	      break;
	    case 0x2f:
	      evo_str += "Leaf Stone";
	      break;
	  }
	evo_reader.get_byte();
	evo_str += " into " + get_pokemon_name(evo_reader.get_byte());
	break;
      case 3:
	evo_str += " evolves by trading";
	evo_reader.get_byte();
	evo_str += " into " + get_pokemon_name(evo_reader.get_byte());
	break;
    }
    if (evo_reader.peek_byte() == 0) more_evolutions = false;
    else if (more_evolutions) evo_str += ", or";
  }

  return evo_str;
}
