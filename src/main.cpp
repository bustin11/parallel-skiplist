
#include "problems.h"
#include "../skiplist.h"

#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <string>
#include <vector>

struct mainArgs_t {
    int help;
    int numThreads;
    float p;
} mainArgs;


void print_usage () {
    puts("Usage: ./main [-h]"
        "  -h         Print this help message.\n\n"
        "Options:\n"
        "  -h           Print this help message.\n"
        "  -N <Threads> Number of threads to launch\n\n"
        "  -p <prob>    Probability of Insertion\n\n"
        "DESCRIPTION:\n"
        "benchmarks the concurrent skiplist implementation"
        "on 2 different axes: \n\n"
        "1) problem size: size of the skiplist, ie, how well\n"
        "performance scales as the size of the skiplist grows\n\n"
        "2) # of threads: how much parallelism (a property of the\n"
        "algorithm itself, describing simultaneous execution) and\n"
        "concurrency (a property of the semantics of the algorithm\n"
        "that logically executes simultaneously) scales with the #\n"
        "of threads. If an application doesn't scale thread-wise\n"
        "what's the point of having multiprocessor machines?\n\n"
        );
}


int main(int argc, char** argv) {

    int opt;
    mainArgs.numThreads = 1;
    mainArgs.p = .5;
    while ((opt = getopt(argc, argv, "hN:p:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'N':
                mainArgs.numThreads = atoi(optarg);
                break;
            case 'p':
                mainArgs.p = atof(optarg);
                break;
        }
    }

    std::vector<int> sizes{10000, 20000, 30000};
    printf("Number of threads:%d\n", mainArgs.numThreads);

    printf("Problem size: \n");
    for (int size : sizes) {
        prob_size(mainArgs.numThreads, size);
    }
    printf("Done.\n");

    printf("Problem size (mixed): \n");
    for (int size : sizes) {
        prob_size_mixed(mainArgs.numThreads, size, mainArgs.p);
    }

    printf("Done.\n");
}