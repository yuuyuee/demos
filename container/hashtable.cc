#include <iostream>
#include <functional>
#include <memory>
#include <iterator>

// hashtable<
//      typename Key,
//      * CopyConstructible type
//
//      typename Value,
//      * CopyConstructible type
//
//      typename Alloc,
//      * allocator<Value>
//
//      typename ExtractKey,
//      * Key Functor(Value)
//      typename Equal,
//      * Compare(Key, Key) = true if Key0 == Key1
//
//      typename H1,
//      * unary hash function
//      * size_t H1(Key) = [0, numeric_limits<size_t>::max()]
//
//      typename H2,
//      * binary range-hashing function
//      * size_t H2(size_t, size_t) = H2(r, n) = [0, N]
//
//      typename Hash,
//      * binary ranged hash function, size_t Hash(Key, size_t)
//      * size_t Hash(Key, size_t) = Hash(k, N) = h2(h1(k), N)
//      * if Hash is anything other than the default, H1 and H2 are ignored.
//
//      typename RehashPolicy,
//      * Policy class with three members.
//      * 0) _M_next_bkt(n): return a bucket count no smaller than n.
//      * 1) _M_bkt_for_elements(n): return a bucket count appropriate for an element count of n.
//      * 2) _M_need_rehash(n_bkt, n_elt, n_ins): determines whether
//          if the current bucket count is n_bkt and the current element cout is n_elt,
//          we need to increase the bucket count. If so, returns make_pair(true, n)
//          where n is the new bucket countm If not, returns make_pair(fasle, <anything>)
//
//      typename Traits
// >


int main(void) {

    return 0;
}
