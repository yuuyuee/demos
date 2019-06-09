Boost Libraries - Chrono Sublibrary
===================================

- Classes:
    class duration<typename Rep, Class Period = radio<1>>;
    typedef duration<boost::int_least64_t, nano> nanoseconds;
    typedef duration<boost::int_least64_t, micro> microseconds;
    typedef duration<boost::int_least64_t, milli> milliseconds;
    typedef duration<boost::int_least64_t> seconds;
    typedef duration<boost::int_least64_t, ratio<60>> minutes;
    typedef duration<boost::int_least64_t, ratio<3600>> hours;
    IO
    class duration_value<Rep>::zero();
    class duration_value<Rep>::min();
    class duration_value<Rep>::max();

    class time_point;
    IO

    class system_clock;
    class steady_clock;//if defined BOOST_CHRONO_HAS_CLOCK_STEADY
    class high_resolution_clock;
    IO

    class process_real_cpu_clock;
    class process_user_cpu_clock;
    class process_system_cpu_clock;
    IO

    class thread_clock; //BOOST_CHRONO_HAS_THREAD_CLOCK
                        //BOOST_CHRONO_THREAD_CLOCK_IS_STEADY
    IO
