#include <cstdint>
#include <windows.h>
#include <iostream>
#include <string>
#include "bitstream.h"
#include "data_structs.h"
#include "pokechar.h"
#include "image_decompression.cpp"
#include "pages.cpp"
using namespace std;
using namespace bitstream;

void (*page_drawers[])(pokemon_data_structure*) = {&page0, &page1};

void jump_to(int line, int column) {
  cout << "\u001b[0m\u001b[" << to_string(line) << ";" << to_string(column) << "H";
}

void clear_area(int startx, int starty, int width, int height) {
  for (int i=0; i<height; i++) {
    int next_line = startx+i;
    jump_to(next_line, starty);
    for (int j=0; j<width; j++) cout << " ";
  }  
  cout << "\u001b[0m" << '\n';
}

void draw_letter(file_bitstream_reader &rom, char letter, int line, int column) {
  rom.seek(0x11a80 + letter*8);

  string fg_chars[] = {"\u001b[38;5;0m", "\u001b[38;5;15m"};
  string bg_chars[] = {"\u001b[48;5;0m", "\u001b[48;5;15m"};

  bool pixels[16];
  for (int i=0; i<4; i++) {
    int next_line = line+i;
    jump_to(next_line, column);
    for (int j=0; j<16; j++) {
      pixels[j] = rom.get();
    }

    for (int j=0; j<8; j++) {
      if (!pixels[j] && !pixels[j+8]) {
        cout << "\u001b[0m\u001b[49m ";
        continue;
      }
      if (!pixels[j] && pixels[j+8]) {
        cout << "\u001b[49m\u001b[38;5;15m\u2584";
        continue;
      }
      if (pixels[j] && !pixels[j+8]) {
        cout << "\u001b[49m\u001b[38;5;15m\u2580";
        continue;
      }
      cout << fg_chars[pixels[j]] << bg_chars[pixels[j+8]] << "\u2580";
    }
  }
}

void draw_string(file_bitstream_reader &rom, string str, int line, int column) {
  int xpos = 0;
  for (int i=0; i<str.length(); i++) {
    char c = str[i];
    if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) draw_letter(rom, c-0x41, line, column + xpos*8);
    else if ('0' <= c && c <= '9')                        draw_letter(rom, c+0x46, line, column + xpos*8);
    else if (c == '-')                                    draw_letter(rom,   0x63, line, column + xpos*8);
    else if (c == '%')                                    draw_letter(rom,   0x61, line, column + xpos*8);
    else if (c == '^')                                    draw_letter(rom,   0x62, line, column + xpos*8);
    else if (c == '.')                                    draw_letter(rom,   0x68, line, column + xpos*8);
    else if (c == '\'')                                   draw_letter(rom,   0x60, line, column + xpos*8);
    else if (c == 'm')                                    draw_letter(rom,   0x62, line, column + xpos*8);
    else if (c == '^')                                    draw_letter(rom,   0x62, line, column + xpos*8);
    else if (c == (char)0x80)                             draw_letter(rom,   0x75, line, column + xpos*8);
    else if (c == (char)0x82)                             draw_letter(rom,   0x75, line, column + xpos*8);
    else                                                  xpos--;
    xpos++;
  }
}

void draw_pokemon_sprite(file_bitstream_reader &rom, uint8_t id, int line, int column) {
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

  jump_to(line, column);
  for (int i=0; i<28; i++) {
    jump_to(line+i, column);
    for (int j=0; j<56; j++) cout << fg_chars[0] << bg_chars[0] << "\u2580";
  }
  line += (56-height)/2;
  column += (56-width)/2;

  uint8_t chars[112];
  for (int i=0; i<height/2; i++) {
    int next_line = line+i;
    jump_to(next_line, column);

    for (int j=0; j<width*2; j++) {
      bool lo_bit = lo_bitplane.get();
      bool hi_bit = hi_bitplane.get();
      chars[j] = (hi_bit << 1) | lo_bit;
    }

    for (int j=0; j<width; j++) {
      cout << fg_chars[chars[j]] << bg_chars[chars[j+width]] << "\u2580";
    }
  }

  free(pointer);
}

void draw_horizontal_line(int line, int start, int length) {
  cout << "\u001b[" << to_string(line) << ";" << to_string(start) << "H";
  for (int i=0; i<length; i++) cout << "-";
}

void draw_vertical_line(int column, int start, int height) {
  cout << "\u001b[" << to_string(start) << ";" << to_string(column) << "H";
  for (int i=0; i<height; i++) cout << "|\u001b[" << to_string(start+i+1) << ";" << to_string(column) << "H";
}

void write_string(string str, int startx, int starty, int width, int height) {
  string lines[100];
  for (int line_num=0; line_num < height; line_num++) {
    jump_to(starty+line_num, startx);
    int line_length = 0;
    for (int i=0; i<width; i++)
      if (str[i] == ' ' || str[i] == 0) line_length = i;
    
    cout << str.substr(0, line_length);
    if (str.length() > line_length) str = str.substr(line_length+1, str.length()-line_length);
    else break;
  }
}

void display_page(pokemon_data_structure *pokemon, int page_num) {
  clear_area(57, 6, 20*8-57, 7*8-6);
  (*page_drawers[page_num])(pokemon);
}
