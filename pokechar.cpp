#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include "bitstream.h"
using namespace std;
using namespace bitstream;

string charset[] = {
"*", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "_", "PKMN", "_", "_", "#", "_", "#", "*", "_", "<player>", "<rival>", "POKé", "_", "……", "_", "_", "<target>", "<attacker>", "PC", "TM", "TRAINER", "ROCKET", ".", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " ", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "(", ")", ":", ";", "[", "]", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "é", "'d", "'l", "'s", "'t", "'v", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "'", "PK", "MN", "-", "'r", "'m", "?", "!", ".", "ァ", "ゥ", "ェ", ">", ">", "V", "♂", "$", "x", ".", "/", ",", "♀", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

string bin_to_chars(file_bitstream_reader &rom, int len) {
  string s;
  for (int i=0; i<len; i++) {
    uint8_t add = rom.get_byte();
    if (add == 0x50) break;
    s += charset[add];
  }
  return s;
}

vector<uint8_t> chars_to_bin(string s) {
  vector<uint8_t> vec;
  for (int i=0; i<s.length(); i++) {
    int c = (int)s[i];

    bool encodable = ((int)'A' <= c  && c <= (int)'Z') || ((int)'a' <= c  && c <= (int)'z');
    if (encodable) {
      c -= (int)'A';
      c += 0x80;
      vec.push_back(c);
    }
    else {
      string ch; ch.push_back(s[i]);
      throw invalid_argument("Cannot convert " + ch + " into binary: converting ~[A-Za-z] not supported yet");
    }
  }

  return vec;
}

string get_pokemon_name(uint8_t id) {
  file_bitstream_reader rom("pkred_rom.gb");
  rom.seek(0x1c21e + ((id-1)*10));
  return bin_to_chars(rom, 10);
}

uint8_t get_pokemon_dexnum(uint8_t id) {
  file_bitstream_reader rom("pkred_rom.gb");
  rom.seek(0x41024 + (id-1));
  return rom.get_byte();
}
