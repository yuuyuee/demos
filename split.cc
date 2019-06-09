// Author: yuyue (X3130@njsecnet.com)

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void SplitRecord(const std::string& record,
                        std::vector<std::string>* res) {
    std::vector<std::string> fields;
    std::string::size_type start = record[0] == '@' ? 1 : 0, end = 0;
    while ((end = record.find_first_of("\t,", start)) != std::string::npos) {
        std::string field(record.data() + start, end - start);
        fields.push_back(std::move(field));
        start = end + 1;
    }
    if (start < record.size()) {
        std::string field(record.data() + start, record.size() - start);
        fields.push_back(std::move(field));
    }
    res->swap(fields);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << argv[0] << " filename" << std::endl;
    return 0;
  }
  return 0;
}
