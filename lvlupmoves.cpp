#include "bitstream.h"
#include "pokechar.h"
#include "constants.h"
#include <string>
#include <tuple>
#include <vector>
using namespace bitstream;
using namespace file_constants;

vector<tuple<uint8_t, uint8_t>> lvlupmoveset(uint8_t id) {
  file_bitstream_reader move_reader(rom_filename);
  vector<tuple<uint8_t, uint8_t>> ret;

  move_reader.seek(0x3b05a + 2*id);
  move_reader.seek(move_reader.get_word() + (0xd << 14));

  while (move_reader.get_byte() != 0) {}

  while (move_reader.peek_byte() != 0) {
    ret.push_back(tuple<uint8_t, uint8_t>(move_reader.get_byte(), move_reader.get_byte()));
  }

  return ret;
}
