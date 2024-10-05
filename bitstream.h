#ifndef BITSTREAM
#define BITSTREAM
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

namespace bitstream {
  class file_bitstream_reader {
  public:
    ifstream bytestream;

    file_bitstream_reader(string filename);

    void close(); 

    bool peek();

    bool get();

    uint8_t peek_byte();

    uint8_t get_byte();

    uint16_t get_word(bool little_endian = true);

    void seek(int address, uint8_t bit = 0);

  private:
    uint8_t byte_pointer;
    uint8_t current_byte;
  };

  class bitstream_reader {
  public:
    uint8_t *read_start;
    int offset;

    bitstream_reader(uint8_t *_read_start);

    bool peek();

    bool get();

    uint8_t peek_byte();

    uint8_t get_byte();

    void seek(int address, uint8_t bit = 0);

  private:
    uint8_t byte_pointer;
  };

  class bitstream_writer {
  public:
    uint8_t *write_start;
    int offset;
    int length;

    bitstream_writer(uint8_t *_write_start, int _length, bool clear_mem = false);

    void write(bool bit);

    void write_byte(uint8_t byte);

    void seek(int address, uint8_t bit = 0);

    bool past_end();

  private:
    uint8_t byte_pointer;
  };

  class image_bitstream_writer {
  public:
    uint8_t *write_start;
    uint8_t width_pixels;
    uint8_t height_pixels;
    int x;
    int y;

    image_bitstream_writer(uint8_t *_write_start, uint8_t width, uint8_t height);

    void write_pair(bool a, bool b);

    bool past_end();

  private:
    uint8_t byte_pointer;
  };
}
#endif
