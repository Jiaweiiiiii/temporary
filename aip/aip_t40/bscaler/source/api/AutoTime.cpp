#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <sys/time.h>
#endif
#include "AutoTime.hpp"

Timer::Timer() {
    reset();
}

Timer::~Timer() {
    // do nothing
}

void Timer::reset()
{
#if defined(_MSC_VER)
    LARGE_INTEGER time, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time);
    uint64_t sec = time.QuadPart / freq.QuadPart;
    uint64_t usec = (time.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart;
    mLastResetTime = sec * 1000000 + usec;
#else
    struct timeval Current;
    gettimeofday(&Current, nullptr);
    mLastResetTime = Current.tv_sec * 1000000 + Current.tv_usec;
#endif
}

uint64_t Timer::durationInUs() {
#if defined(_MSC_VER)
    LARGE_INTEGER time, freq;
    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);
    uint64_t sec = time.QuadPart / freq.QuadPart;
    uint64_t usec = (time.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart;
    auto lastTime = sec * 1000000 + usec;
#else
    struct timeval Current;
    gettimeofday(&Current, nullptr);
    auto lastTime = Current.tv_sec * 1000000 + Current.tv_usec;
#endif

    return lastTime - mLastResetTime;
}

AutoTime::AutoTime(int line, const char* func) : Timer() {
    mName = ::strdup(func);
    mLine = line;
}

AutoTime::~AutoTime() {
    auto timeInUs = durationInUs();
    if (!access("/tmp/aie_debug", F_OK)) {
        printf("%s, %d, cost time: %f ms\n", mName, mLine, (float)timeInUs / 1000.0f);
    }
    free(mName);
}
