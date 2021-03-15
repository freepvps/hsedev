#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>

int get_time_now() {
    time_t now = time(NULL);

    struct timeval now_us;
    gettimeofday(&now_us, NULL);

    struct timespec now_ns;
    clock_gettime(CLOCK_REALTIME, &now_ns);

    printf("%lld\n", (long long int)now);
    printf("%lld %d\n", (long long int)now_us.tv_sec, (int)now_us.tv_usec);
    printf("%lld %d\n", (long long int)now_ns.tv_sec, (int)now_ns.tv_nsec);
    return 0;
}

int time_print() {
    char buf[20];
    time_t t = time(NULL);
    struct tm now_desc = *localtime(&t);
    strftime(buf, sizeof(buf), "%F %X", &now_desc);
    if (strptime(buf, "%F %X", &now_desc) == NULL) {
        perror("strptime");
        return -1;
    }
    time_t t2 = mktime(&now_desc);
    printf("%s\n", buf);
    printf("%d\n", t == t2);
    printf("%d\n", now_desc.tm_isdst);
    return 0;
}

int set_file_time(int argc, char** argv) {
    struct stat s;
    s.st_atim.tv_sec = 1;
    stat(argv[1], &s);
    struct utimbuf t;
}


int main(int argc, char** argv) {
    // time_print();
    set_file_time(argc, argv);
    return 0;
}
