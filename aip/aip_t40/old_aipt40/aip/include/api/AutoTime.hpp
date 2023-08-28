#ifndef __AUTOTIME_HPP__
#define __AUTOTIME_HPP__

#include <stdint.h>
#include <stdio.h>

#if __GNUC__ < 5 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 7)
#define constexpr const
const class nullptr_t
{
public:
    template<class T>
    inline operator T*() const { return 0; }
    template<class C, class T>
    inline operator T C::*() const
        { return 0; }
private:
    void operator&() const;
} nullptr = {};
#endif

class Timer {
public:
    Timer();
    ~Timer();
    Timer(const Timer&)  = delete;
    Timer(const Timer&&) = delete;
    Timer& operator=(const Timer&)  = delete;
    Timer& operator=(const Timer&&) = delete;

    // reset timer
    void reset();
    // get duration (us) from init or latest reset.
    uint64_t durationInUs();

protected:
    uint64_t mLastResetTime;
};

/** time tracing util. prints duration between init and deinit. */
class AutoTime : Timer {
public:
    AutoTime(int line, const char* func);
    ~AutoTime();
    AutoTime(const AutoTime&)  = delete;
    AutoTime(const AutoTime&&) = delete;
    AutoTime& operator=(const AutoTime&) = delete;
    AutoTime& operator=(const AutoTime&&) = delete;

private:
    int mLine;
    char* mName;
};
#ifndef RELEASE
#define AUTOTIME AutoTime ___t(__LINE__, __func__)
#else
#define AUTOTIME
#endif

#endif /* __AUTOTIME_HPP__ */
