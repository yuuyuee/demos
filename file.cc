// Author: yuyue/X3130 (yuyue2200@hotmail.com)
// File: file.cc

#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

const char* KeyName(int index) {
  static const char* key_name[] = {
      NULL, "Mime type", "File name", "Payload length"
  };
  switch (index) {
  case 1:
  case 2:
  case 3:
    return key_name[index];
  default:
    return NULL;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "file xxx.fhmf" << std::endl;
    return 0;
  }
  const char* fname = argv[1];
  std::ifstream in(fname);
  if (!in) {
    std::cout << "Can't read from " << fname << std::endl;
    return -1;
  }

  /// Read file header
  static const char* kFileMagic = "FHMF";
  static const size_t kFileHeaderLen = 8;

  char fhdr[kFileHeaderLen];
  memset(fhdr, 0, kFileHeaderLen);
  in.read(fhdr, kFileHeaderLen);
  if (memcmp(fhdr, kFileMagic, strlen(kFileMagic))) {
    std::cout << "Unknown format file" << std::endl;
    return -1;
  }

  static const size_t kVersionOffset = 4;
  static const size_t kFileCountOffset = 6;
  unsigned short version = ntohs(*reinterpret_cast<unsigned short*>(fhdr + kVersionOffset));
  unsigned short fcount = ntohs(*reinterpret_cast<unsigned short*>(fhdr + kFileCountOffset));
  std::cout << "Version: " << version << ", file count: " << fcount << std::endl;

  /// Read field 0/1
  static const size_t kFieldHeaderSize = 23;
  static const size_t kKeyCountOffset = 22;
  static const char* kFieldMagic = "\r\n\r\n----------------\r\n";

  for (int i = 0; i < fcount; ++i) {
    // Read Field header
    const char* field = i == 0 ? "Request" : "Response";
    char field_hdr[kFieldHeaderSize];
    memset(field_hdr, 0, kFieldHeaderSize);
    in.read(field_hdr, kFieldHeaderSize);
    assert(!memcmp(field_hdr, kFieldMagic, strlen(kFieldMagic)));
    // 1bytes
    int key_count = field_hdr[kKeyCountOffset];
    std::cout << field << " key count: " << key_count << std::endl;

    // Read attributes
    int payload_len = 0;
    for (int j = 0; j < key_count; ++j) {
      char attr[2] = {0 /* key type*/, 0 /* key length */};
      in.read(attr, sizeof(attr));

      const char* key_type = KeyName(attr[0]);
      int key_len = attr[1];

      std::string key(key_len, 0);
      in.read(&(key[0]), key_len);

      std::cout << field << " " << key_type << "(" << key_len << "): " << key << std::endl;

      if (j == 2)
        payload_len = std::stoi(key);
    }

    // Read split
    static const char* kSplitMagic = "\r\n\r\n";
    static const size_t kSplitSize = 4;
    char split[kSplitSize];
    memset(split, 0, kSplitSize);
    in.read(split, kSplitSize);
    assert(!memcmp(split, kSplitMagic, kSplitSize));

    // Read payload
    assert(payload_len >= 0);
    if (payload_len > 0) {
      char payload[payload_len];
      memset(payload, 0, payload_len);
      in.read(payload, payload_len);
      std::cout.write(payload, payload_len) << std::endl;
    }
  }

  return 0;
}
