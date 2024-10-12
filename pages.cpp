#include "data_structs.h"
#include "draw.h"

void page0(pokemon_data_structure *pokemon) {
  write_string("----------------TM USAGE---------------", 59, 7, 100, 1);
  write_string("----------------HM USAGE---------------", 59, 35, 100, 1);
  for (int i=0; i<50; i++) {
    string ability = (pokemon->tm_usage[i] ? "\u001b[32mABLE" : "\u001b[31mNOT ABLE");
    int line_num = i%25 + 9;
    int col_num = 61 + (i>=25)*20;
    write_string("TM" + to_string(i+1) + ": " + ability + "\u001b[0m", col_num, line_num, 25, 1);
  }
  for (int i=0; i<5; i++) {
    string ability = (pokemon->tm_usage[i] ? "\u001b[32mABLE" : "\u001b[31mNOT ABLE");
    write_string("HM" + to_string(i+1) + ": " + ability + "\u001b[0m", 61, 37+i, 25, 1);
  }
}

void page1(pokemon_data_structure *pokemon) {

}
