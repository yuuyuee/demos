// Author: yuyue/X3130 (yuyue2200@hotmail.com)
// File: algorithm1.cc

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

template<typename Container>
void Dump(Container const& c) {
  for (const auto& e: c)
    std::cout << e << ' ';
  std::cout << std::endl;
}

int main() {
  /* adjacent_find, all_of (C++0x), any_of (C++0x), none_of (C++0x),
    copy, copy_backward, copy_if (C++0x), copy_n (C++0x), count, count_if
    equal, equal_range
    fill, fill_n,
    binary_search, search, search_n,
    find, find_end, find_first_of, find_if, find_if_not (C++0x)
    for_each, generate, generate_n
    includes
    inplace_merge
    is_heap (C++0x), is_heap_until (C++0x), pop_heap, push_heap, make_heap, sort_heap
    is_partitioned (C++0x)
    is_sorted (C++0x), is_sorted_until (C++0x), sort, stable_sort
    iter_swap
    lexicographical_compare
    lower_bound, upper_bound

    max, max_element, min, min_element, minmax (C++0x), minmax_element (C++0x)
    merge
    mismatch

    next_permutation, prev_permutation

    nth_element

    partial_sort,  partial_sort_copy
    partition, stable_partition, partition_copy (C++0x), partition_point (C++0x)

    random_shuffle

    remove, remove_copy, remove_copy_if, remove_if

    replace, replace_copy, replace_copy_if, replace_if

    reverse, reverse_copy

    rotate, rotate_copy

    set_difference, set_intersection, set_symmetric_difference, set_union
    shuffle (C++0x)

    swap, swap_ranges

    transform

    unique, unique_copy */
  {
    std::vector<int> iv{1, 0, 2, 2, 3 ,5 ,5 ,8};
    std::vector<int> res;
    std::adjacent_difference(iv.begin(), iv.end(), std::back_inserter(res));
  }
  return 0;
}


