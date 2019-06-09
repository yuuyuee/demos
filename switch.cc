#include <iostream>

enum class Role { DROP = 1, FORWARD = 2, SAMPLED = 4 };

void Case(Role role) {
  switch (role) {
  case Role::DROP: {
    std::cout << "Role::DROP" << std::endl;
    break;
  }

  case Role::FORWARD: {
    std::cout << "Role::FORWARD" << std::endl;
    break;
  }

  case Role::SAMPLED: {
    std::cout << "Role::SAMPLED" << std::endl;
    break;
  }

  default:
    std::cout << "unknown" << std::endl;
    break;
  }
}

int main() {
  Case(Role::FORWARD);
  return 0;
}
