#include <iostream>
#include <exception>
#include <new>

template<typename _Tp>
class new_allocator {
public:
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef _Tp*            pointer;
    typedef const _Tp*      const_pointer;
    typedef _Tp&            reference;
    typedef const _Tp&      const_reference;
    typedef _Tp             value_type;

    new_allocator() noexcept {}
    explicit new_allocator(const new_allocator&) noexcept {}

    template<typename _Tp1>
        new_allocator(const new_allocator<_Tp1> &) noexcept {}
    ~new_allocator() noexcept {}

    pointer address(reference x) const noexcept {
        return reinterpret_cast<pointer>(&x);
    }
    const_pointer address(const_reference x) const noexcept {
        return reinterpret_cast<const_pointer>(&x);
    }

    size_type max_size(void) noexcept {
        return std::size_t(-1) / sizeof(_Tp);
    }

    pointer allocate(size_type n, const void * = 0) {
        if (n > this->max_size())
            throw std::bad_alloc();
        return static_cast<pointer>(::operator new(n * sizeof(_Tp)));
    }

    void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }
#if __cplusplus >= 201103L
    template<typename _Up, typename... _Args>
        void construct(_Up *p, _Args&&... args) {
            ::new((void *)p) _Up(args...);
        }

    template<typename _Up>
        void destroy(_Up *p) {
            p->~_Up();
        }

#else
    void construct(pointer p, const _Tp& v) {
        ::new((void *)p) _Tp(v);
    }

    void destroy(pointer p) {
        p->~_Tp();
    }
#endif
};

template<typename _Tp>
class allocator: public new_allocator<_Tp> {
public:
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef _Tp*            pointer;
    typedef const _Tp*      const_pointer;
    typedef _Tp&            reference;
    typedef const _Tp&      const_reference;
    typedef _Tp             value_type;

    template<typename _Tp1>
    struct rebind {
        typedef allocator<_Tp1> other;
    };

    allocator() noexcept {}
    allocator(const allocator& a) noexcept:new_allocator<_Tp>(a) {}
    template<typename _Tp1>
    allocator(const allocator<_Tp1>&) noexcept {}
    ~allocator() noexcept {}
};

int main(void) {
    class test {
    public:
        test() {
            std::cout << "class test construct" << std::endl;
        }
        ~test() {
            std::cout << "class test destruct" << std::endl;
        }
    };


    // new_allocator test
    new_allocator<int> alloc;
    int *p = nullptr;
    p = alloc.allocate(5);
    for (int i = 0; i < 5; i++)
        p[i] = i;
    for (int i = 0; i < 5; i++)
        std::cout << p[i] << std::endl;
    alloc.deallocate(p, 5);

    //allocator test
    allocator<char> alloc1;
    char *p1 = alloc1.allocate(5);
    for (int i = 0; i < 5; i++)
        p1[i] = '0' + i;
    for (int i = 0; i < 5; i++)
        std::cout << p1[i] << std::endl;
    alloc1.deallocate(p1, 5);

    return 0;
}
