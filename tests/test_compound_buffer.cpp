#include <fstream>
#include <sstream>

#undef NDEBUG
#include <cassert>

#include "nbt.hpp"

#include <filesystem>

#include <cstdlib>
#include <memory>

int main() {

  const size_t file_size = std::filesystem::file_size("compound.nbt");

  assert(("file_size>0", file_size > 0));

  const size_t buffer_len = file_size * 1.5;
  void* const bufferA = malloc(buffer_len);
  assert(("bufferA!=nullptr", bufferA != nullptr));
  void* const bufferB = malloc(buffer_len);
  assert(("bufferB!=nullptr", bufferB != nullptr));

  memset(bufferA, 0, buffer_len);
  memset(bufferB, 0, buffer_len);

  void* bufferA_cur = bufferA;
  void* const bufferA_end = (void*) ((uint8_t*) bufferA + buffer_len);
  void* bufferB_cur = bufferB;
  void* const bufferB_end = (void*) ((uint8_t*) bufferB + buffer_len);

  // decode from file and encode to bufferB
  nbt::NBT(std::ifstream("compound.nbt", std::ios::binary))
      .encode(&bufferB_cur, bufferB_end);


  const size_t encodeB_bytes = (uint8_t*) bufferB_cur - (uint8_t*) bufferB;
  assert(("encodeB_bytes==file_size", encodeB_bytes == file_size));
  std::ofstream("compound_bufferB.nbt", std::ios::binary)
      .write((char*) bufferB, encodeB_bytes);

  // load file into bufferA
  {
    std::ifstream is("compound.nbt", std::ios::binary);

    assert(("bool(is)", bool(is)));

    is.readsome((char*) bufferA, file_size);
    is.close();
  }
  // decode from bufferA and encode into bufferA
  nbt::NBT(bufferA, bufferA_end).encode(&bufferA_cur, bufferA_end);

  const size_t encodeA_bytes = (uint8_t*) bufferA_cur - (uint8_t*) bufferA;

  assert(("encodeA_bytes==file_size", encodeA_bytes == file_size));

  std::ofstream("compound_bufferA.nbt", std::ios::binary)
      .write((char*) bufferA, encodeA_bytes);

  for(size_t i = 0; i < encodeA_bytes; i++) {
    assert(("bufferA[i]==bufferB[i]",
        reinterpret_cast<uint8_t*>(bufferA)[i] ==
            reinterpret_cast<uint8_t*>(bufferB)[i]));
  }


  free(bufferA);
  free(bufferB);
  return 0;
}
