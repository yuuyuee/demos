#include <iostream>
#include <string>
#include <iterator>

#include <vector>
#include <list>
#include <forward_list>
#include <array>
#include <deque>

#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <queue>
#include <stack>

std::string iterator_name(std::input_iterator_tag cate) {
    return "input_iterator_tag";
}

std::string iterator_name(std::output_iterator_tag cate) {
    return "output_iterator_tag";
}

std::string iterator_name(std::forward_iterator_tag cate) {
    return "forward_iterator_tag";
}

std::string iterator_name(std::bidirectional_iterator_tag cate) {
    return "bidirectional_iterator_tag";
}

std::string iterator_name(std::random_access_iterator_tag cate) {
    return "random_access_iterator_tag";
}

#define DUMP_CONTAINER_CATEGORY(Iter) do {                                      \
    std::cout << #Iter << " - "                                                 \
              << iterator_name(std::iterator_traits<Iter>::iterator_category()) \
              << std::endl;                                                     \
} while (0)

int main(void) {
    DUMP_CONTAINER_CATEGORY(std::vector<int>::iterator);
    DUMP_CONTAINER_CATEGORY(std::list<int>::iterator);
    DUMP_CONTAINER_CATEGORY(std::forward_list<int>::iterator);
    //DUMP_CONTAINER_CATEGORY(std::array<int, 100>::iterator);
    DUMP_CONTAINER_CATEGORY(std::deque<int>::iterator);
#if 0
    DUMP_CONTAINER_CATEGORY(std::set<int>);
    DUMP_CONTAINER_CATEGORY(std::multiset<int>);
    DUMP_CONTAINER_CATEGORY(std::unordered_set<int>);
    DUMP_CONTAINER_CATEGORY(std::map<int, int>);
    DUMP_CONTAINER_CATEGORY(std::multimap<int, int>);
    DUMP_CONTAINER_CATEGORY(std::unordered_map<int, int>);
    DUMP_CONTAINER_CATEGORY(std::queue<int>);
    DUMP_CONTAINER_CATEGORY(std::priority_queue<int>);
    DUMP_CONTAINER_CATEGORY(std::stack<int>);
#endif
    return 0;
}
