#include <cstdio>
#include <algorithm>
#include <cstring>
#include <windows.h>
#include <cstdlib>
#include <string>
#include "bitstream.h"
#include "data_structs.h"
#include "pokechar.h"
#include "draw.h"
#include "pokemon_data_conversion.cpp"
#include "constants.h"
using namespace std;
using namespace pokemon_constants;
using namespace file_constants;

int get_id_from_name(file_bitstream_reader &rom, char *nme) {
  if (!strcmp("nidoranm", nme)) return 3;
  if (!strcmp("nidoranf", nme)) return 15;

  string name = nme;
  for (int i=0; i<name.length(); i++) if ('a'<=name[i] && name[i]<='z') name[i] -= 'a'-'A';

  int ret = 0;

  while (ret <= 190) {
    rom.seek(pokename_pointer + (ret++)*10);
    string readname = read_string(rom);
    if (readname==name) return ret;
  }
  
  return NULL;
}

int main(int argc, char *argv[]) {

  system("");
  cout << "\x1b[0m";

  file_bitstream_reader rom(rom_filename);

  id = get_id_from_name(rom, argv[1]);
  if (id == NULL) {
    cout << "No POKéMON named " << static_cast<string>(argv[1]) << " found in database!" << endl;
    return 0;
  }
  
  dexnum = get_pokemon_dexnum(id);
  if (dexnum == 0 || dexnum == 151) return 0;

  data_struct = extract_pokemon_data(dexnum);
  dexinfo = extract_dex_data(id);

  string dexnum_string = "%^";  // Placeholders for PK and MN sprites
  for (int i=to_string(dexnum).length(); i<3; i++) dexnum_string += "0";
  dexnum_string += to_string(dexnum);

  string name = "";
  name += get_pokemon_name(id);

  int typestringlen = 1;

  string type1string;
  cout << to_string(data_struct->type1);

  rom.seek(typename_pointer + (2*data_struct->type1));
  rom.seek(rom.get_word() + 0x20000);
  type1string = read_string(rom);
  typestringlen += type1string.length();
  type1string = "\x1b[38;5;" + to_string(TYPE_COLORS[data_struct->type1]) + "m" + type1string;

  string type2string;
  rom.seek(typename_pointer + (2*data_struct->type2));
  rom.seek(rom.get_word() + 0x20000);
  type2string = read_string(rom);
  typestringlen += type2string.length();
  type2string = "\x1b[38;5;" + to_string(TYPE_COLORS[data_struct->type2]) + "m" + type2string;

  clear_area(6, 1, 56, 56);
  clear_area(1, 1, 29*8+1, 4);
  draw_horizontal_line(5, 1, WIDTH*8);
  draw_vertical_line(57, 6, HEIGHT*8 - 8);
  draw_string(rom, dexnum_string, 1, 1);
  write_string(type1string + '/' + type2string + "\x1b[0m", 55-typestringlen, 36, 80, 1);
  draw_string(rom, name, 1, (WIDTH-name.length())*4+1);
  draw_pokemon_sprite(rom, data_struct, 6, 1);
  write_string("ID: #" + to_string(id), 3, 36, 10, 1);

  write_string("Height: " + to_string(dexinfo->height_feet) + "' " + to_string(dexinfo->height_inches) + '"', 3, 38, 20, 1);
  write_string("Weight: " + to_string(dexinfo->weight_tenthpounds/10) + '.' + to_string(dexinfo->weight_tenthpounds%10) + "lb", 44-to_string(dexinfo->weight_tenthpounds).length(), 38, 20, 1);

  rom.seek(dexinfo->dexentry_pointer + 0xac001-0x4000);
  string dexstring = read_string(rom, 1000);
  replace(dexstring.begin(), dexstring.end(), '_', ' ');
  write_string(dexstring, 3, 40, 50, 10);

  write_string(dexinfo->pkmnkind, (58-strlen(dexinfo->pkmnkind))/2, 36, 12, 1);

  write_string("HP: " + to_string(data_struct->base_hp), 3, 44, 20, 1);
  write_string("Attack: " + to_string(data_struct->base_attack), 3, 45, 20, 1);
  write_string("Defence: " + to_string(data_struct->base_defence), 3, 46, 20, 1);
  write_string("Speed: " + to_string(data_struct->base_speed), 3, 47, 20, 1);
  write_string("Special: " + to_string(data_struct->base_special), 3, 48, 20, 1);

  string basehp_meter = "";
  for (int i=0; i<data_struct->base_hp; i+=8) basehp_meter += "#";
  write_string(basehp_meter, 22, 44, 33, 1);

  string baseattack_meter = "";
  for (int i=0; i<data_struct->base_attack; i+=8) baseattack_meter += "#";
  write_string(baseattack_meter, 22, 45, 33, 1);

  string basedefence_meter = "";
  for (int i=0; i<data_struct->base_defence; i+=8) basedefence_meter += "#";
  write_string(basedefence_meter, 22, 46, 33, 1);

  string basespeed_meter = "";
  for (int i=0; i<data_struct->base_speed; i+=8) basespeed_meter += "#";
  write_string(basespeed_meter, 22, 47, 33, 1);

  string basespecial_meter = "";
  for (int i=0; i<data_struct->base_special; i+=8) basespecial_meter += "#";
  write_string(basespecial_meter, 22, 48, 33, 1);

  rom.close();
  cout << "\x1b[0m";
  
  int pagenum = 0;
  while (pagenum < NUM_PAGES) {
    display_page(data_struct, pagenum++);
    // Keep the application running until all pages have been seen
    getchar();
  }
  free(data_struct);

  cout << "\x1b[2J";
  jump_to(1, 1);
  return 0;
}
