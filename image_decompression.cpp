#include "bitstream.h"
#include <cstddef>
#include <cstdint>
using namespace std;
using namespace bitstream;

void decompBitplane(file_bitstream_reader &rom, uint8_t *bitplane, uint8_t width_pixels, uint8_t height_pixels);
void delta_decode(uint8_t *bitplane, uint8_t width_pixels, uint8_t height_pixels);
void xor_buffers(uint8_t *base1, uint8_t *base2, uint8_t width_pixels, uint8_t height_pixels);

const int BYTES_PER_PLANE = 8 * 7 * 7; // max pokemon front sprite size in Pokémon Gen 1 is 7x7

// Pointer to bitplanes of size BYTES_PER_PLANE on heap that hold sprite data
// Bitplane 0 is low bit, bitplane 1 is high bit
// Caller is responsible for freeing this data
uint8_t *decompress_image(file_bitstream_reader &rom)
{
  uint8_t dimensions = rom.get_byte();
  uint8_t width_pixels = (dimensions & 0xF0) >> 4;
  uint8_t height_pixels = dimensions & 0x0F;

  width_pixels *=8;  height_pixels *=8; 

  bool swap = rom.get();

  uint8_t *all_buffers = (uint8_t*)calloc(2, BYTES_PER_PLANE);
  if (all_buffers == NULL) cout << "Space not available on heap ;(";

  uint8_t *baseA = all_buffers;
  uint8_t *baseB = baseA + BYTES_PER_PLANE;
  
  uint8_t *base1;
  uint8_t *base2;
  if (!swap) {
    base1 = baseA;
    base2 = baseB;
  }
  else {
    base1 = baseB;
    base2 = baseA;
  }
  
  uint8_t mode;
  
  decompBitplane(rom, base1, width_pixels, height_pixels);
  mode = rom.get();
  if (mode == 1) mode = rom.get() + 1;
  decompBitplane(rom, base2, width_pixels, height_pixels);

  switch (mode) {
    case 0:
      delta_decode(base1, width_pixels, height_pixels);
      delta_decode(base2, width_pixels, height_pixels);
      break;
    case 1:
      delta_decode(base1, width_pixels, height_pixels);
      xor_buffers(base2, base1, width_pixels, height_pixels);
      break;
    case 2:
      delta_decode(base1, width_pixels, height_pixels);
      delta_decode(base2, width_pixels, height_pixels);
      xor_buffers(base2, base1, width_pixels, height_pixels);
      break;
  }

  return all_buffers;
}


void decompBitplane(file_bitstream_reader &rom, uint8_t *bitplane, uint8_t width_pixels, uint8_t height_pixels) {
  image_bitstream_writer writer(bitplane, width_pixels, height_pixels);

  bool packet_type = rom.get(); // 0 = RLE, 1 = DATA

  while (!writer.past_end()) {
    if (packet_type == 0) {
      int count = 0;
      int rle_length = 0;

      do {
        rle_length = (rle_length << 1) | rom.get();
        count++;
      } while ((rle_length & 1) == 1);

      int add = 0;
      for (int i=0; i<count; i++) {
        add = (add << 1) | rom.get();
      }

      rle_length += add + 1;
      
      for (int i=0; i<rle_length && !writer.past_end(); i++) {
        writer.write_pair(0, 0);
      }

    } else {
      bool a = rom.get();
      bool b = rom.get();

      while ((a==1 || b==1) && !writer.past_end()){
        writer.write_pair(a, b);
        if (writer.past_end()) break;
        a = rom.get();
        b = rom.get();
      }
    }
    
    packet_type = !packet_type;
  }
}

void delta_decode(uint8_t *buffer, uint8_t width_pixels, uint8_t height_pixels) {
  bitstream_reader reader(buffer);
  bitstream_writer writer(buffer, width_pixels*height_pixels/8);
  bool write_val;

  while (!writer.past_end()) {
    write_val = 0;
    for (int i=0; i<width_pixels; i++) {
      if (reader.get()) write_val = !write_val;
      writer.write(write_val);
    }
  }
}

void xor_buffers(uint8_t *buffer1, uint8_t *buffer2, uint8_t width_pixels, uint8_t height_pixels) {
  bitstream_reader _buffer1(buffer1);
  bitstream_reader _buffer2(buffer2);
  bitstream_writer write_buffer(buffer1, width_pixels * height_pixels / 8);

  while (!write_buffer.past_end()) {
    write_buffer.write(_buffer1.get() ^ _buffer2.get());
  }
}
