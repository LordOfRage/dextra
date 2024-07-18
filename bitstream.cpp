#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include "bitstream.h"
using namespace std;

namespace bitstream {

  file_bitstream_reader::file_bitstream_reader(string filename) {
    bytestream.open(filename, ios::binary);
    byte_pointer = 0;
    current_byte = bytestream.get();
  }

  void file_bitstream_reader::close() {
    bytestream.close();
  }

  bool file_bitstream_reader::peek() {
    return (current_byte & (128 >> byte_pointer)) != 0;
  }

  bool file_bitstream_reader::get() {
    bool returnval = (current_byte & (128 >> byte_pointer++)) != 0;
    if (byte_pointer == 8) {
      byte_pointer = 0;
      current_byte = bytestream.get();
    }
    return returnval;
  }

  uint8_t file_bitstream_reader::peek_byte() {
    return current_byte;
  }

  uint8_t file_bitstream_reader::get_byte() {
    byte_pointer = 0;
    uint8_t returnval = peek_byte();
    current_byte = bytestream.get();
    return returnval;
  }

  uint16_t file_bitstream_reader::get_word(bool little_endian) {
    uint8_t byte1 = get_byte();
    uint8_t byte2 = get_byte();

    if (little_endian) {
      return (byte2 << 8) | byte1;
    }
    else {
      return (byte1 << 8) | byte2;
    }
  }

  void file_bitstream_reader::seek(int address, uint8_t bit) {
    bytestream.seekg(address);
    byte_pointer = bit;
    current_byte = bytestream.get();
  }

  bitstream_reader::bitstream_reader(uint8_t *_read_start) {
    read_start = _read_start;
    byte_pointer = 0;
    offset = 0;
  }

  bool bitstream_reader::peek() {
    return (read_start[offset] & (128 >> byte_pointer)) != 0;
  }

  bool bitstream_reader::get() {
    bool returnval = (read_start[offset] & (128 >> byte_pointer++)) != 0;
    if (byte_pointer == 8) {
      byte_pointer = 0;
      offset++;
    }
    return returnval;
  }

  uint8_t bitstream_reader::peek_byte() {
    return read_start[offset];
  }

  uint8_t bitstream_reader::get_byte() {
    byte_pointer = 0;
    uint8_t returnval = peek_byte();
    offset++;
    return returnval;
  }

  void bitstream_reader::seek(int address, uint8_t bit) {
    offset = address;
    byte_pointer = bit;
  }

  bitstream_writer::bitstream_writer(uint8_t *_write_start, int _length, bool clear_mem) {
    write_start = _write_start;
    length = _length;
    byte_pointer = 0;
    offset = 0;
    if (clear_mem) {
      for (int i=0; i<length; i++) {
        write_start[i] = 0; 
      }
    }
  }

  void bitstream_writer::write(bool bit) {
    if (bit) {
      write_start[offset] |= (128 >> byte_pointer);
    }
    else {
      write_start[offset] &= 255 ^ (128 >> byte_pointer);
    }
    byte_pointer++;
    if (byte_pointer == 8) {
      byte_pointer = 0;
      offset++;
    }
  }

  void bitstream_writer::write_byte(uint8_t byte) {
    write_start[offset++] = byte;
  }

  void bitstream_writer::seek(int address, uint8_t bit) {
    offset = address;
    byte_pointer = bit;
  }

  bool bitstream_writer::past_end() {
    return offset >= length;
  }

  image_bitstream_writer::image_bitstream_writer(uint8_t *_write_start, uint8_t width, uint8_t height) {
    write_start = _write_start;
    width_pixels = width;
    height_pixels = height;
    byte_pointer = 0;
    x = 0;
    y = 0;
  }

  void image_bitstream_writer::write_pair(bool a, bool b) {
    //a=1; b=1;
    write_start[width_pixels/8*y + x/8] |= (128*a >> byte_pointer);
    write_start[width_pixels/8*y + x/8] |= (64*b >> byte_pointer);
    y++;
    if (y == height_pixels) {
      y = 0;
      byte_pointer += 2;
      if (byte_pointer == 8) {
        byte_pointer = 0;
        x += 8;
      }
    }

  }

  bool image_bitstream_writer::past_end() {
    return x >= width_pixels;
  }
}
