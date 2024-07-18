#include <cstdint>
#include <stdexcept>
#include <string>
using namespace std;

char hex_vals[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

int linear_search(char c)
{
  for (int i=0; i<0x10; i++) {
    if (hex_vals[i] == c) {
      return i;
    }
  }

  return 0x10;
}

string dec_to_hex(int denary)
{
  string hex;
  while (denary > 0) {
    hex = hex_vals[denary & 0xF] + hex;
    denary >>= 4;
  }
  
  if (hex.length() == 0) {
    hex = "0";
  }
  return hex;
}

uint8_t hex_to_dec(string hex) {
  uint8_t ret = 0;
  for (char c : hex) {
    uint8_t digit = linear_search(c);
    if (digit == 0x10) {
      throw invalid_argument("Invalid hex value");
    }
    else {
      ret += digit;
      ret <<= 4;
    }
  }

  return ret;
}
