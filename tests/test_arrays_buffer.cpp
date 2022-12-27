#include <fstream>
#include <sstream>

#undef NDEBUG
#include <cassert>

#include "nbt.hpp"

int main() {

  using std::cout, std::endl;

  nbt::TagByteArray test_byte_array {0, 1, 2, 3};
  nbt::TagIntArray test_int_array {0, 1 << 16, 2 << 16, 3 << 16};
  nbt::TagLongArray test_long_array {0, 1LL << 32, 2LL << 32, 3LL << 32};

  nbt::NBT root {"Array Test",
      {
          {"byte array", test_byte_array},
          {"int array", test_int_array},
          {"long array", test_long_array},
      }};

  const size_t buffer_len = 4096;
  void* const buffer = malloc(buffer_len);
  assert(("buffer!=nullptr", buffer != nullptr));

  void* buffer_cur = buffer;
  void* const buffer_end = (void*) ((uint8_t*) buffer + buffer_len);

  root.encode(&buffer_cur, buffer_end);
  // write buffer to file
  /*
    {
      std::ofstream ofile("arrays_from_buffer.nbt", std::ios::binary);

      if(!ofile) {
        cout << "Failed to create arrays_from_buffer.nbt" << endl;
        ofile.close();
        return 1;
      }

      ofile.write((char*) buffer, (char*) buffer_cur - (char*) buffer);
      ofile.close();
    }
  */
  cout << "encode finished with " << (uint8_t*) buffer_cur - (uint8_t*) buffer
       << " bytes." << endl;

  buffer_cur = buffer;
  nbt::NBT mem;
  mem.decode(buffer, buffer_end);


  assert(("test_byte_arry",
      root.data->tags.at("byte array") == mem.data->tags.at("byte array")));

  assert(("test_int_array",
      root.data->tags.at("int array") == mem.data->tags.at("int array")));

  assert(("test_long_array",
      root.data->tags.at("long array") == mem.data->tags.at("long array")));

  free(buffer);
  return 0;
}