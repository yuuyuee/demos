
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>


void ReadFile(const char* fname, std::string* buffer) {
  char tmp[1024];
  std::ifstream in(fname);
  if (fname) {
    ssize_t n = 0;
    while ((n = in.readsome(tmp, 1024)) > 0)
      buffer->append(tmp, n);
  }
}


bool UTF8_1bytes(const uint8_t* s) {
  return /* *s >= 0x00 && */
         *s <= 0x7F;
}

bool UTF8_2bytes(const uint8_t* s) {
  return (s[0] >= 0xC2 && s[0] <= 0xDF) &&
         (s[1] >= 0x80 && s[1] <= 0xBF);
}

bool UTF8_3bytes(const uint8_t* s) {
  switch (s[0]) {
  case 0xE0:
    return (s[1] >= 0xA0 && s[1] <= 0xBF) &&
           (s[2] >= 0x80 && s[2] <= 0xBF);
    break;
  case 0xE1:
  case 0xE2:
  case 0xE3:
  case 0xE4:
  case 0xE5:
  case 0xE6:
  case 0xE7:
  case 0xE8:
  case 0xE9:
  case 0xEA:
  case 0xEB:
  case 0xEC:
    return (s[1] >= 0x80 && s[1] <= 0xBF) &&
           (s[2] >= 0x80 && s[2] <= 0xBF);
    break;
  case 0xED:
    return (s[1] >= 0x80 && s[1] <= 0x9F) &&
           (s[2] >= 0x80 && s[2] <= 0xBF);
    break;
  case 0xEE:
  case 0xEF:
    return (s[1] >= 0x80 && s[1] <= 0xBF) &&
           (s[2] >= 0x80 && s[2] <= 0xBF);
    break;
  default:
    return false;
  }
}

bool UTF8_4bytes(const uint8_t* s) {
  switch (s[0]) {
  case 0xF0:
    return (s[1] >= 0x90 && s[1] <= 0xBF) &&
           (s[2] >= 0x80 && s[2] <= 0xBF) &&
           (s[3] >= 0x80 && s[3] <= 0xBF);
    break;
  case 0xF1:
  case 0xF2:
  case 0xF3:
    return (s[1] >= 0x80 && s[1] <= 0xBF) &&
           (s[2] >= 0x80 && s[2] <= 0xBF) &&
           (s[3] >= 0x80 && s[3] <= 0xBF);
    break;
  case 0xF4:
    return (s[1] >= 0x80 && s[1] <= 0x9F) &&
           (s[2] >= 0x80 && s[2] <= 0xBF) &&
           (s[3] >= 0x80 && s[3] <= 0xBF);
    break;
  default:
    return false;
  }
}

bool UTF8_CheckImpl(const uint8_t* s, size_t n) {
  while (n > 0) {
    if ((*s & 0xF0) == 0xF0) {
      if (n < 4 || !UTF8_4bytes(s))
        return false;
      s += 4;
      n -= 4;
      continue;
    }

    if ((*s & 0xE0) == 0xE0) {
      if (n < 3 || !UTF8_3bytes(s))
        return false;
      s += 3;
      n -= 3;
      continue;
    }

    if ((*s & 0xC0) == 0xC0) {
      if (n < 2 || !UTF8_2bytes(s))
        return false;
      s += 2;
      n -= 2;
      continue;
    }

    if (UTF8_1bytes(s)) {
      s += 1;
      n -= 1;
      continue;
    }

    return false;
  }
  return true;
}

bool UTF8_Check(const void* s, size_t n) {
  return UTF8_CheckImpl(reinterpret_cast<const uint8_t*>(s), n);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " filename" << std::endl;
    return -1;
  }

  std::string buffer;
  ReadFile(argv[1], &buffer);
  if (UTF8_Check(buffer.data(), buffer.size())) {
    std::cout << argv[1] << " is UTF-8 encode" << std::endl;
  } else {
    std::cout << argv[1] << " is not UTF-8 encode" << std::endl;
  }

  return 0;
}
