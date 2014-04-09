
#include <stdlib.h>

#if defined(WIN32)

#include <winsock.h>
#include <sys\timeb.h>
int gettimeofday(struct timeval *tp, void *tzp);
#define usleep(mseconds) Sleep(mseconds/1000)

#define M_PI 3.14159

#else

#include <unistd.h>
#include <sys/time.h>

#endif


// Return the time in sec. in a double value.
double Wallclock(void);

// Block for a given number of seconds (a double value)
void Wait(double);

// Returns a random value within the interval [min, max]
float frand(float min, float max);
