#include <iostream>
#include <chrono>

class CallOnceEverySecond {
public:
    CallOnceEverySecond(int sec)
        : sec_(sec), flag_(false) {}
    ~CallOnceEverySecond() = default;

    template<typename Func>
    void operator()(int sec, Func&& func) {
        if (sec == sec_ && !flag_) {
            func();
            flag_ = true;
        } else if (sec != sec_) {
            flag_ = false;
        }
    }

private:
    int sec_;
    int flag_;
};

class Now {
public:
    Now() = default;
    ~Now() = default;
    void operator()() {
        time_t timestamp = time(nullptr);
        struct tm tm;
        localtime_r(&timestamp, &tm);
        std::cout << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec << std::endl;
    }
};

int main() {
    CallOnceEverySecond call_once(10);
    while (true) {
        time_t timestamp = time(nullptr);
        struct tm tm;
        localtime_r(&timestamp, &tm);
        call_once(tm.tm_sec, Now());
    }
    return 0;
}
