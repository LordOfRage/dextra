#include <fstream>
#include <windows.h> 
#include <cstdint>
#include <cstdlib>
#include <string>
#include "bitstream.h"
#include "data_structs.h"
#include "pokechar.h"
#include "draw.h"
#include "pokemon_data_conversion.cpp"
#include "constants.h"
using namespace std;

int main(int argc, char *argv[]) {
  
//  for (int i=0; i<argc; i++) {
//    cout << argv[i] << "\n";
//  }

  system("");
  cout << "\u001b[0m";

  file_bitstream_reader rom(rom_filename);
  ofstream debug("DEBUG.txt");
  const int WIDTH = 20;
  const int HEIGHT = 7;

  int id = stoi(argv[1]);

  uint8_t dex = get_pokemon_dexnum(id);
  //if (dex == 0 || dex == 151) return 0;

  pokemon_data_structure *pokemon = extract_pokemon_data(dex);

  string dexnum_string = "%^";  // Placeholders for PK and MN sprites
  for (int i=to_string(dex).length(); i<3; i++) dexnum_string += "0";
  dexnum_string += to_string(dex);

  string name = "";
  //for (int i=get_pokemon_name(id).length(); i<10; i++) name += "-";
  name += get_pokemon_name(id);

  int type_colors[] = {15, 208, 172, 129, 136, 243, 0, 10, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 196, 21, 28, 190, 207, 4, 166};

  string type1string;
  rom.seek(0x27dae + (2*pokemon->type1));
  rom.seek(rom.get_word() + 0x20000);
  type1string = "\u001b[38;5;" + to_string(type_colors[pokemon->type1]) + "m" + read_string(rom);

  string type2string;
  rom.seek(0x27dae + (2*pokemon->type2));
  rom.seek(rom.get_word() + 0x20000);
  type2string = "\u001b[38;5;" + to_string(type_colors[pokemon->type2]) + "m" + read_string(rom);

  clear_area(6, 1, 56, 56);
  clear_area(1, 1, 29*8+1, 4);
  draw_horizontal_line(5, 1, WIDTH*8);
  draw_vertical_line(57, 6, HEIGHT*8 - 6);
  draw_string(rom, dexnum_string, 1, 1);
  write_string(type1string + '/' + type2string + "\u001b[0m", 3, 40, 80, 1);
  //draw_string(rom, "--------------", 1, 41);
  draw_string(rom, name, 1, (WIDTH-name.length())*4+1);
  draw_pokemon_sprite(rom, id, 6, 1);
  write_string("ID: #" + to_string(id), 3, 36, 10, 1);
  display_page(pokemon, 0);

  free(pokemon);
  rom.close();
  cout << "\u001b[0m";
  jump_to(WIDTH*8, HEIGHT*8);
  
  while (!getchar()) {
    // Keep the application running until manual exit
  }

  cout << "\u001b[2J";
  jump_to(1, 1);
  return 0;
}
