
#include "problems.h"
#include "skiplist.h"

#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <string>
#include <vector>

struct mainArgs_t {
    int help;
    int numThreads;
    int testID;
} mainArgs;


void print_usage () {
    puts("Usage: ./main [-h] -T <numThreads> -n <testNumber>\n" "Options:\n"
        "  -h         Print this help message.\n"
        "  -N <num>   Number of threads to launch (check your hardware for cores).\n"
        "  -T <num>   Test Number (1-12)\n"
        "    - 1-6 test insertions, with increasing difficulty to pass\n"
        "    - 7-12 test removal, with increasing difficulty to pass\n"
        );
}


int main(int argc, char** argv) {

    int opt;
    mainArgs.numThreads = 1;
    mainArgs.testID = 0;
    while ((opt = getopt(argc, argv, "hT:N:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'T':
                mainArgs.testID = atoi(optarg);
                break;
            case 'N':
                mainArgs.numThreads = atoi(optarg);
                break;
        }
    }

    prob1(mainArgs.numThreads);

    printf("All Tests Passed!\n");
}