//
// Created by Haifa Bogdan Adnan on 13/08/2018.
//

#include "win64_compatibility_layer.h"

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if (NULL != tv)
    {
        GetSystemTimeAsFileTime(&ft);

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tmpres /= 10;  /*convert into microseconds*/
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    return 0;
}