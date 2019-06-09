#include <iostream>
#include <cassert>
#include <new>

namespace __new_delete_function_declare {
#if 0
// normal version new/delete
void* operator new(std::size_t) throw(std::bad_alloc);
void* operator new[](std::size_t) throw(std::bad_alloc);
void operator delete(void*) noexcept;
void operator delete[](void*) noexcept;

void* operator new(std::size_t, std::nothrow_t&) noexcept;
void* operator new[](std::size_t, std::nothrow_t&) noexcept;
void operator delete(void*, std::nothrow_t&) noexcept;
void operator delete[](void*, std::nothrow_t&) noexcept;

// Placement version new/delete
inline void* operator new(std::size_t, void* p) noexcept { return p; }
inline void* operator new[](std::size_t, void* p) noexcept { return p; }
inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}

// new expression
// ::/*optional*/ new(placement)/*optional*/ (type) initializer_list/*optional*/;
// ::/*optional*/ new(placement)/*optional*/ type initializer_list/*optional*/;
// ::/*optional*/ delete expression;
// ::/*optional*/ delete [] expression;
#endif
} // namespace __new_delete_function_declare

int main(void) {
    struct Type {
        Type(): val(0) { std::cout << "constructor" << std::endl; }
        ~Type() { std::cout << "destructor" << std::endl; }
        int val;
    };

    // maybe throw exception
    void* mem0 = ::operator new(sizeof(Type));
    // no exception
    void* mem1 = ::operator new(sizeof(Type), std::nothrow);
    assert(mem1 != 0);

    // replacement new examples
    int* i0 = static_cast<int*>(::operator new(sizeof(int), mem0));
    *i0 = 100;
    assert(*i0 == *(int*)mem0);
    // do nothings
    ::operator delete(i0, mem0);

    Type* i1 = new(mem1) Type;
    // XXX: do nothings ???
    delete i1;
    std::cout << "\'delete type\' has be call destroy function only in that by placement new." << std::endl;

    ::operator delete(mem0);
    ::operator delete(mem1, std::nothrow);

    return 0;
}
