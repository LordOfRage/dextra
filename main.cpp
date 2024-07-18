#include <windows.h> 
#include <cstdint>
#include <cstdlib>
#include <string>
#include "bitstream.h"
#include "image_decompression.cpp"
#include "pokechar.cpp"
using namespace std;

void draw_pokemon_sprite(file_bitstream_reader &rom, uint8_t id) {
  string fg_chars[] = {"\u001b[38;5;15m", "\u001b[38;5;250m", "\u001b[38;5;243m", "\u001b[38;5;0m"};
  string bg_chars[] = {"\u001b[48;5;15m", "\u001b[48;5;250m", "\u001b[48;5;243m", "\u001b[48;5;0m"};

  uint8_t dex = get_pokemon_dexnum(id);

  rom.seek(0x0383E8 + (dex-1)*28);

  int dimensions = rom.get_byte();
  int height = (dimensions & 0xf0) >> 1;
  int width = (dimensions & 0x0f) << 3;

  int bank;
  if (id == 0x15)                       bank = 0x1;
  else if (id == 0xb6)                  bank = 0xb;
  else if (0x00 <= id && id <= 0x1e)    bank = 0x9;
  else if (0x1f <= id && id <= 0x49)    bank = 0xa;
  else if (0x4a <= id && id <= 0x73)    bank = 0xb;
  else if (0x74 <= id && id <= 0x98)    bank = 0xc;
  else                                  bank = 0xd;
  int offset = rom.get_word();


  rom.seek((bank << 14) + (offset & 0x3fff));
  uint8_t *pointer = decompress_image(rom);
  bitstream_reader lo_bitplane(pointer);
  bitstream_reader hi_bitplane(pointer + BYTES_PER_PLANE);

  uint8_t chars[112];
  for (int i=0; i<height/2; i++) {
    for (int j=0; j<width*2; j++) {
      bool lo_bit = lo_bitplane.get();
      bool hi_bit = hi_bitplane.get();
      chars[j] = (hi_bit << 1) | lo_bit;
    }

    for (int j=0; j<width; j++) {
      cout << fg_chars[chars[j]] << bg_chars[chars[j+width]] << "\u2580";
    }
    cout << "\u001b[0m";
    cout << '\n';
  }

  free(pointer);
}

int main(int argc, char *argv[]) {
  
//  for (int i=0; i<argc; i++) {
//    cout << argv[i] << "\n";
//  }

  system("");

  file_bitstream_reader rom("pkred_rom.gb");

  for (uint8_t i=0; i<0xbe; i++) {
    if (get_pokemon_dexnum(i+1) == 0 || get_pokemon_dexnum(i+1) == 151) continue;
    draw_pokemon_sprite(rom, i+1);
    Sleep(500);
  }

  rom.close();
  return 0;
}
