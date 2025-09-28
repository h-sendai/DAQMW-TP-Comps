#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "timespecsub.h"

int debug = 0;

int usage()
{
    char msg[] = "Usage: decode-latency [-D] [-d] [-h] filename\n"
                 "-D: debug\n"
                 "-d: dump data\n"
                 "-h: help\n";
    fprintf(stderr, "%s", msg);

    return 0;
}

int main(int argc, char *argv[])
{
    int do_dump = 0;
    int c;
    while ( (c = getopt(argc, argv, "Ddh")) != -1) {
        switch (c) {
            case 'D':
                debug = 1;
                break;
            case 'd':
                do_dump = 1;
                break;
            case 'h':
                usage();
                exit(0);
            default:
                usage();
                exit(1);
        }
    }

    argc -= optind;
    argv += optind;

    if (argc != 1) {
        usage();
        exit(1);
    }

    FILE *fp = fopen(argv[0], "r");
    if (fp == NULL) {
        err(1, "fopen");
    }

    unsigned char buf[sizeof(unsigned long) + 2*sizeof(struct timespec) + sizeof(unsigned long)];

    int n_data = 0;
    for ( ; ; ) {
        int n = fread(buf, 1 /* byte */, sizeof(buf), fp);
        if (n == 0) {
            if (feof(fp)) {
                break;
            }
            else {
                err(1, "fread");
            }
        }
        else if (n != sizeof(buf)) {
            errx(0, "short read: %d bytes", n);
        }
    
        n_data ++;
        struct timespec start;

        unsigned long *data_size_p = (unsigned long *) &buf[0];
        unsigned long data_size = *data_size_p;
        
        struct timespec *ts_sender_p = (struct timespec *) &buf[sizeof(unsigned long)];
        struct timespec *ts_logger_p = (struct timespec *) &buf[sizeof(unsigned long) + sizeof(struct timespec)];

        struct timespec ts_sender = *ts_sender_p;
        struct timespec ts_logger = *ts_logger_p;
        
        unsigned long *n_unread_buf_p = (unsigned long *) &buf[sizeof(unsigned long) + 2*sizeof(struct timespec)];
        unsigned long n_unread_buf = *n_unread_buf_p;

        if (do_dump) {
            printf("%ld.%09ld %ld.%09ld %.3f %ld\n",
                ts_sender.tv_sec, ts_sender.tv_nsec,
                ts_logger.tv_sec, ts_logger.tv_nsec,
                data_size/1024.0,
                n_unread_buf
            );
        }
        else {
            if (n_data == 1) {
                start = ts_sender;
            }

            struct timespec diff, elapsed;
            timespecsub(&ts_logger, &ts_sender, &diff);
            timespecsub(&ts_sender, &start, &elapsed);
            printf("%ld.%09ld %ld.%09ld %.3f kB %ld\n",
                elapsed.tv_sec, elapsed.tv_nsec, diff.tv_sec, diff.tv_nsec, data_size/1024.0, n_unread_buf);
        }
        
    }


    return 0;
}
