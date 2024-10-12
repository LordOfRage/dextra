#include "bitstream.h"
#include "data_structs.h"
#include <cstdint>
using namespace bitstream;

#ifndef DRAW
#define DRAW

void jump_to(int, int);
void clear_area(int, int, int, int);
void draw_letter(file_bitstream_reader&, char, int, int);
void draw_string(file_bitstream_reader&, string, int, int);
void draw_pokemon_sprite(file_bitstream_reader&, uint8_t, int, int);
void draw_horizontal_line(int, int, int);
void draw_vertical_line(int, int, int);
void write_string(string, int, int, int, int);
void display_page(pokemon_data_structure*, int);

#endif
