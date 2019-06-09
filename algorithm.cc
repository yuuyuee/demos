#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <set>

template <typename T> struct Display: public std::unary_function<T, void> {
    typename std::unary_function<T, void>::result_type
        operator()(typename std::unary_function<T, void>::argument_type const& arg) {
        std::cout << arg << " ";
    }
};

int main(void) {
    int ia[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> iv1(ia, ia + 5);
    std::vector<int> iv2(ia, ia + 9);
    std::vector<int> iv3(ia + 5, ia + 10);

    std::equal_to<int> equal_to;
    std::cout << std::equal(std::begin(iv1), std::end(iv1), std::begin(iv1)) << std::endl;
    std::cout << std::equal(iv1.cbegin(), iv1.cend(), iv1.cbegin(), equal_to) << std::endl;

    std::for_each(ia, ia + 10, Display<int>()); std::cout << std::endl;
    std::fill(ia, ia + 10, 100);
    std::for_each(ia, ia + 10, Display<int>()); std::cout << std::endl;

    std::for_each(iv1.cbegin(), iv1.cend(), Display<int>()); std::cout << std::endl;
    std::for_each(iv3.cbegin(), iv3.cend(), Display<int>()); std::cout << std::endl;
    //std::iter_swap(iv1.begin(), iv3.begin());
    std::swap_ranges(iv1.begin(), iv1.end(), iv3.begin());
    std::for_each(iv1.cbegin(), iv1.cend(), Display<int>()); std::cout << std::endl;
    std::for_each(iv3.cbegin(), iv3.cend(), Display<int>()); std::cout << std::endl;

    std::vector<int> tmp1(5, 0);
    std::vector<int> tmp2(5, 0);
    std::copy(iv1.begin(), iv1.end(), tmp1.begin());
    std::copy_backward(iv1.begin(), iv1.end(), tmp2.end());
    std::for_each(tmp1.cbegin(), tmp1.cend(), Display<int>()); std::cout << std::endl;
    std::for_each(tmp2.cbegin(), tmp2.cend(), Display<int>()); std::cout << std::endl;
    std::for_each(tmp2.cbegin(), tmp2.cend(), [](const int& x) {
        std::cout << x << " ";
    });
    std::cout << std::endl;

    std::cout << "Adjacent_find ";
    auto iter = std::adjacent_find(iv1.begin(), iv2.end(), [](const int& a, const int& b) {
        return a - 1 == b;
    });
    std::cout << *iter << std::endl;

    std::cout << "count "<< std::count(iv1.begin(),  iv1.end(), 3) << std::endl;
    std::cout << "count_if arguments ";
    std::for_each(iv1.cbegin(), iv1.cend(), Display<int>()); std::cout << std::endl;
    std::cout << "count_if "<< std::count_if(iv1.begin(),  iv1.end(), [](const int& v) { return v % 2 == 0;}) << std::endl;

    std::vector<int> tmp3(5, 0);
    struct Gen {
        int operator()() { return 100; }
    };
    std::generate(tmp3.begin(), tmp3.end(), Gen());
    std::cout << "generate ";
    std::for_each(tmp3.cbegin(), tmp3.cend(), Display<int>()); std::cout << std::endl;
    std::generate_n(tmp3.begin(), tmp3.size(), []() { return 200; });
    std::cout << "generate_n ";
    std::for_each(tmp3.cbegin(), tmp3.cend(), Display<int>()); std::cout << std::endl;

    std::set<int> s1({1, 2, 3, 4, 5, 6}), s2({3, 4, 5});
    std::cout << "includes " << std::includes(s1.begin(), s1.end(), s2.begin(), s2.end()) << std::endl;

    std::vector<int> s3(s1.size() + s2.size(), 0);
    std::merge(s1.begin(), s1.end(), s2.begin(), s2.end(), s3.begin());
    std::cout << "merge ";
    std::for_each(s3.cbegin(), s3.cend(), Display<int>()); std::cout << std::endl;

    std::partition(iv2.begin(), iv2.end(), [](const int& v) { return v % 2 == 0; });
    std::for_each(iv2.cbegin(), iv2.cend(), Display<int>()); std::cout << std::endl;

    std::vector<int> iv4({0, 1, 2, 3, 4, 5, 0, 5, 6, 6, 0, 6, 5, 7});
    auto iter1 = std::remove(iv4.begin(), iv4.end(), 0);
    std::cout << "Remove last iterator " << *iter1 << std::endl;
    std::for_each(iv4.cbegin(), iv4.cend(), Display<int>()); std::cout << std::endl;

    std::vector<int> iv5;
    std::remove_copy(iv4.cbegin(), iv4.cend(), std::back_inserter(iv5), 6);
    std::for_each(iv5.cbegin(), iv5.cend(), Display<int>()); std::cout << std::endl;

    std::cout << "Remove_if ";
    std::remove_if(iv5.begin(), iv5.end(), std::bind(std::equal_to<int>(), std::placeholders::_1, 5));
    std::for_each(iv5.cbegin(), iv5.cend(), Display<int>()); std::cout << std::endl;

    std::vector<int> iv6;
    std::transform(iv5.cbegin(), iv5.cend(), std::back_inserter(iv6),
            std::bind(std::multiplies<int>(), std::placeholders::_1, 2));
    std::cout << "transform ";
    std::for_each(iv6.cbegin(), iv6.cend(), Display<int>()); std::cout << std::endl;

    std::vector<int> iv7;
    std::transform(iv5.cbegin(), iv5.cend(), iv6.cbegin(), std::back_inserter(iv7),
            std::bind(std::plus<int>(), std::placeholders::_1, std::placeholders::_2));
    std::cout << "transform ";
    std::for_each(iv7.cbegin(), iv7.cend(), Display<int>()); std::cout << std::endl;
    return 0;
}
