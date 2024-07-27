#include "../defs.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
    #include <time.h>
#endif

// to include sleep function
void sleep_seconds(int seconds) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
}