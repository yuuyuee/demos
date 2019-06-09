#include <iostream>
#include <set>
#include <functional>
#include <memory>
#include <cassert>
#include <iterator>

#include <ctime>
#include <cstdlib>

int Random() {
    static bool initialize = false;
    if (!initialize) {
        initialize = true;
        srand(time(0));
    }
    return random() % 23;
}


template<typename Key, typename Value, typename KeyofValue,
    typename Compare, typename Alloc = std::allocator<Value>>
using rb_tree = std::_Rb_tree<Key, Value, KeyofValue, Compare, Alloc>;

template<typename Tp>
struct identity: public std::unary_function<Tp, Tp> {
    const Tp& operator()(const Tp& x) const {
        return x;
    }
};

template<typename Tp>
void dump(const Tp& v) {
    // typedef _Rb_tree_iterator<value_type> iterator;
    typedef typename Tp::iterator iterator;
    // typedef _Rb_tree_const_iterator<value_type> const_iterator;
    typedef typename Tp::const_iterator const_iterator;
    // typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef typename Tp::reverse_iterator reverse_iterator;
    // typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename Tp::const_reverse_iterator const_reverse_iterator;

    for (const_iterator iter = v.begin(); iter != v.end(); iter++) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

int main() {
    rb_tree<int, int, identity<int>, std::greater<int>> rb;

    for (int i = 0; i < 5; i++) {
        rb._M_insert_equal_(rb.begin(), Random());
    }
    dump(rb);

    // erase operation
    rb.erase(rb.begin());
    dump(rb);

    // inset operation
    int value = Random();
    rb._M_insert_equal_(rb.end(), value);
    dump(rb);

    // find operation
    typedef rb_tree<int, int, identity<int>, std::greater<int>>::iterator iterator;
    iterator iter = rb.find(value);
    std::cout << "Find " << value << ", result " << *iter << std::endl;

    return 0;
}
