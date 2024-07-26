#include <fstream>
#include <windows.h> 
#include <cstdint>
#include <cstdlib>
#include <string>
#include "bitstream.h"
#include "draw.cpp"
using namespace std;

int main(int argc, char *argv[]) {
  
//  for (int i=0; i<argc; i++) {
//    cout << argv[i] << "\n";
//  }

  system("");

  file_bitstream_reader rom("pkred_rom.gb");
  ofstream debug("DEBUG.txt");
  const int WIDTH = 29;

  draw_horizontal_line(5, 1, WIDTH*8);
  draw_vertical_line(57, 6, 50);

  int id = stoi(argv[1]);

  uint8_t dex = get_pokemon_dexnum(id);
  if (dex == 0 || dex == 151) return 0;

  string dexnum_string = "%^";
  for (int i=to_string(dex).length(); i<3; i++) dexnum_string += "0";
  dexnum_string += to_string(dex);

  string name = "";
  //for (int i=get_pokemon_name(id).length(); i<10; i++) name += "-";
  name += get_pokemon_name(id);

  clear_area(6, 1, 56, 56);
  clear_area(1, 1, 29*8+1, 4);
  draw_string(rom, dexnum_string, 1, 1);
  //draw_string(rom, "--------------", 1, 41);
  draw_string(rom, name, 1, (WIDTH-name.length())*4+1);
  draw_pokemon_sprite(rom, id, 6, 1);
  Sleep(500);

  rom.close();
  cout << "\u001b[0m";
  return 0;
}
