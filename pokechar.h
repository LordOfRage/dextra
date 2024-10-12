#include "bitstream.h"
#include <cstdint>
#include <vector>
using namespace std;
using namespace bitstream;

#ifndef POKECHAR
#define POKECHAR

string read_string(file_bitstream_reader&, int=10, uint8_t=0x50);
vector<uint8_t> chars_to_bin(string);
string get_pokemon_name(uint8_t);
uint8_t get_pokemon_dexnum(uint8_t);
uint8_t get_pokemon_id(uint8_t);
string get_pokemon_dex_entry(uint8_t);

#endif
