#include <iostream>
#include <queue>
#include <vector>
#include <deque>
#include <functional>

int main(void) {
    std::priority_queue<int, std::deque<int>, std::greater<int>> queue;
    for (int i = 0; i < 10; i++) {
        queue.push(i);
    }
    while (!queue.empty()) {
        std::cout << queue.top() << " ";
        queue.pop();
    }
    std::cout << std::endl;

    return 0;
}
