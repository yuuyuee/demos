#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main(void) {
    char buffer[1024] = {0};
    string str(buffer);
    str.append("123");
    cout << buffer << endl;
    return 0;
}
