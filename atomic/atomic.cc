#include <iostream>
#include <atomic>

using std::cout;
using std::endl;
using std::atomic_int;

int main(void) {
    atomic_int i(0);
    i++;
    cout << "atomic_int is lock-free: " << i.is_lock_free() << endl;
    cout << i << endl;

    return 0;
}
