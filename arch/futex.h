/* futex.h                                                         -*- C++ -*-
   Jeremy Barnes, 25 January 2012
   Copyright (c) 2012 mldb.ai inc.  All rights reserved.
   This file is part of MLDB. Copyright 2015 mldb.ai inc. All rights reserved.

   Basic futex function wrappers.
*/

#pragma once

#include <atomic>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <limits.h>
#include <time.h>

namespace MLDB {

inline long sys_futex(void *addr1, int op, int val1, struct timespec *timeout = 0, void *addr2 = 0, int val3 = 0)
{
    return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

#if 0
inline void futex_wake(int & futex, int nToWake = INT_MAX)
{
    sys_futex(&futex, FUTEX_WAKE, nToWake, 0, 0, 0);
}

inline void futex_wake(volatile int & futex, int nToWake = INT_MAX)
{
    futex_wake(const_cast<int &>(futex), nToWake);
}
#endif

template<typename T>
inline void futex_wake(std::atomic<T> & futex, int nToWake = INT_MAX)
{
    static_assert(sizeof(std::atomic<T>) == 4, "futex type must be a 32-bit value");
    sys_futex(&futex, FUTEX_WAKE, nToWake, 0, 0, 0);
}

#if 0
inline long futex_wait(const int & futex, int oldValue)
{
    return sys_futex(&const_cast<int &>(futex), FUTEX_WAIT, oldValue, 0, 0, 0);
}
 
inline long futex_wait(volatile const int & futex, int oldValue)
{
    return futex_wait(const_cast<int &>(futex), oldValue);
}
#endif

template<typename T>
inline long futex_wait(std::atomic<T> & futex, int oldValue)
{
    static_assert(sizeof(std::atomic<T>) == 4, "futex type must be a 32-bit value");
    return sys_futex(&futex, FUTEX_WAIT, oldValue, 0, 0, 0);
}

template<typename T>
inline long futex_wait(const std::atomic<T> & futex, int oldValue)
{
    static_assert(sizeof(std::atomic<T>) == 4, "futex type must be a 32-bit value");
    return sys_futex((void *)&futex, FUTEX_WAIT, oldValue, 0, 0, 0);
}

#if 0
inline long futex_wait(const int & futex, int oldValue, double waitTime)
{
    struct timespec timeout;
    timeout.tv_sec = waitTime;
    timeout.tv_nsec = (waitTime - timeout.tv_sec) * 1000000000.0;
    return sys_futex(&const_cast<int &>(futex), FUTEX_WAIT, oldValue, &timeout, 0, 0);
}

inline long futex_wait(volatile const int & futex, int oldValue, double waitTime)
{
    return futex_wait(const_cast<int &>(futex), oldValue, waitTime);
}
#endif

#if 0
template<typename T>
inline long futex_wait(std::atomic<T> & futex, int oldValue, double waitTime)
{
    static_assert(sizeof(std::atomic<T>) == 4, "futex type must be a 32-bit value");
    struct timespec timeout;
    timeout.tv_sec = waitTime;
    timeout.tv_nsec = (waitTime - timeout.tv_sec) * 1000000000.0;
    return sys_futex(&futex, FUTEX_WAIT, oldValue, &timeout, 0, 0);
}
#endif

template<typename T>
inline long futex_wait(const std::atomic<T> & futex, int oldValue, double waitTime)
{
    static_assert(sizeof(std::atomic<T>) == 4, "futex type must be a 32-bit value");
    struct timespec timeout;
    timeout.tv_sec = waitTime;
    timeout.tv_nsec = (waitTime - timeout.tv_sec) * 1000000000.0;
    return sys_futex((void *)(&futex),
                     FUTEX_WAIT, oldValue, &timeout, 0, 0);
}

inline void futex_unlock(void * futex)
{
    std::atomic<int> & lock = *reinterpret_cast<std::atomic<int> *>(futex);
    int newLock = lock.fetch_add(1) + 1;
    if (newLock == 0)
        futex_wake(lock);
}

} // namespace MLDB
