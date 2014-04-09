#include "glutil.h"


double
Wallclock(void)
{
    struct timeval tv;

    gettimeofday(&tv, 0);

    return (((double) tv.tv_sec) + ((double) tv.tv_usec) / 1000000.0);
}

void
Wait(double val)
{
    unsigned long usec = (unsigned long)( val * 1000000.0 );
    usleep( usec );
}

#if defined(WIN32)
int
gettimeofday(struct timeval *tp, void *tzp)
{
	struct _timeb t;

    _ftime(&t);
    tp->tv_sec = t.time;
    tp->tv_usec = t.millitm * 1000;
    return 0;
}
#endif

float frand(float min, float max)
{
        float rr;

#if defined(WIN32)
		rr = (float)rand() / (float)RAND_MAX;
#else
        rr = drand48();
#endif
        return min + rr * (max - min);
}
