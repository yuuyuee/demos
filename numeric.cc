#include <iostream>
#include <vector>
#include <numeric>
#include <functional>

int main(void) {
    int ia[5] = {1, 2, 3, 4, 5};
    std::vector<int> iv(ia, ia + 5);

    std::plus<int> plus;
    std::minus<int> minus;
    std::multiplies<int> multiplies;
    //std::divides<int> divides;
    //std::modulus<int> modulus;

    std::cout << "Accumulate " <<  std::accumulate(iv.cbegin(), iv.cend(), 0) << std::endl;
    std::cout << "Accumulate " << std::accumulate(iv.cbegin(), iv.cend(), 0, minus) << std::endl;

    std::cout << "Inner_product " << std::inner_product(iv.cbegin(), iv.cend(), iv.cbegin(), 1) << std::endl;
    std::cout << "Inner_product " << std::inner_product(iv.cbegin(), iv.cend(), ia, 1, plus, multiplies) << std::endl;

    int ia1[5] = {0};

    std::cout << "Partial_sum ";
    std::partial_sum(iv.cbegin(), iv.cend(), ia1);
    for (int i = 0; i < 5; ++i)
        std::cout << ia1[i] << " ";
    std::cout << std::endl;

    std::cout << "Partial_sum ";
    std::partial_sum(iv.cbegin(), iv.cend(), ia1, multiplies);
    for (int i = 0; i < 5; ++i)
        std::cout << ia1[i] << " ";
    std::cout << std::endl;

    std::cout << "Adjacent_difference ";
    std::adjacent_difference(iv.cbegin(), iv.cend(), ia1);
    for (int i = 0; i < 5; ++i)
        std::cout << ia1[i] << " ";
    std::cout << std::endl;

    std::cout << "Adjacent_difference ";
    std::adjacent_difference(iv.cbegin(), iv.cend(), ia1, plus);
    for (int i = 0; i < 5; ++i)
        std::cout << ia1[i] << " ";
    std::cout << std::endl;

    return 0;
}
