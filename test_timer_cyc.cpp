#include <iostream>
#include <cstdint>
#include <chrono>

inline uint64_t rdtsc() {
    unsigned int lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#include <sys/time.h>


inline double GetTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec / 1000000;
}
int main() {
    uint64_t start, end;

    start = rdtsc();
    double s = 0; 
    double t = 0;
    uint64_t c = 0;
#define type3

    for(int i = 0; i < 10000; i++) {
#ifdef type1
        auto time_point1 = std::chrono::high_resolution_clock::now();
        auto time_point2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = time_point2 - time_point1;
        s += elapsed.count();
#elif defined(type2)
        auto t1 = GetTime();
        auto t2 = GetTime();
        t += t2 - t1;
#else
        auto c1 = rdtsc();
        auto c2 = rdtsc();
        c += c2 - c1;
#endif
    }

    end = rdtsc();

#ifdef type1
    std::cout << s << std::endl;
#elif defined(type2)
    std::cout << t << std::endl;
#else 
    std::cout << c << std::endl;
#endif

    uint64_t cycles = end - start;

    std::cout << "Cycles taken by std::chrono::high_resolution_clock::now(): " << cycles << std::endl;

    return 0;
}

