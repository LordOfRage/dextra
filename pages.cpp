#include <cstdint>
#include <string>
#include "bitstream.h"
#include "data_structs.h"
#include "draw.h"
#include "evolution.cpp"
#include "lvlupmoves.cpp"
#include "constants.h"
#include "pokechar.h"

string get_move_name(uint8_t id) {
  file_bitstream_reader name_finder(rom_filename);
  name_finder.seek(0xb0000);
  while (--id) {
    while (name_finder.get_byte() != 0x50) {}
  }

  return read_string(name_finder, 12);
}

void page0(pokemon_data_structure *pokemon) {
  write_string("------------LEVEL UP MOVESET-----------", 59, 7, 100, 1);
  vector<tuple<uint8_t, uint8_t>> moves = lvlupmoveset(get_pokemon_id(pokemon->dexnum));
  string movestr = "";
  for (auto move : pokemon->initial_attacks) if (move != 0) movestr += "---: " + get_move_name(move) + "\n";

  for (auto move : moves) movestr += "LVL" + to_string(get<0>(move)) + ": " + get_move_name(get<1>(move)) + "\n";

  write_string(movestr, 59, 9, 30, 30);

  write_string("----------------TM USAGE---------------", 119, 7, 100, 1);
  write_string("----------------HM USAGE---------------", 119, 35, 100, 1);
  for (int i=0; i<50; i++) {
    string ability = (pokemon->tm_usage[i] ? "\x1b[32mABLE" : "\x1b[31mNOT ABLE");
    int line_num = i%25 + 9;
    int col_num = 61 + (i>=25)*20 + 60;
    write_string("TM" + to_string(i+1) + ": " + ability + "\x1b[0m", col_num, line_num, 25, 1);
  }
  for (int i=0; i<5; i++) {
    string ability = (pokemon->hm_usage[i] ? "\x1b[32mABLE" : "\x1b[31mNOT ABLE");
    write_string("HM" + to_string(i+1) + ": " + ability + "\x1b[0m", 121, 37+i, 25, 1);
  }
}

void page1(pokemon_data_structure *pokemon) {
	write_string(evo_info(get_pokemon_id(pokemon->dexnum)), 59, 7, 20, 20);
}
