#include <iostream>
#include <iterator>
#include <vector>
#include <list>

/**
 * std::iterator_traits<Tp>::difference_type
 * std::iterator_traits<TP>::value_type
 * std:;iterator_traist<Tp>::pointer
 * std:;iterator_traist<Tp>::reference
 * std:;iterator_traist<Tp>::iterator_category
 */
template<typename Iterator>
void __dump_vector(Iterator first, Iterator last,
                   std::random_access_iterator_tag) {
    std::cout << "Random access iterator" << std::endl;
}

template<typename Iterator>
void __dump_vector(Iterator first, Iterator last,
                   std::bidirectional_iterator_tag) {
    std::cout << "Bidirectional iterator" << std::endl;
}

template <typename Iterator>
void dump_vector(Iterator first, Iterator last) {
    __dump_vector(first, last,
    typename std::iterator_traits<Iterator>::iterator_category());
}


int main(void) {
    std::vector<int> iv{1, 2, 3, 4};
    std::list<int> il{1, 2, 3, 4};

    dump_vector(iv.begin(), iv.end());
    dump_vector(il.begin(), il.end());

    return 0;
}
