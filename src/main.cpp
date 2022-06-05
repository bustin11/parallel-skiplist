
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
        "  -p <prob>    Probability of Insertion\n\n"
        "DESCRIPTION:\n"
        "The higher the probability, the slower it runs since we could try\n"
        "removing elements that doesn't exist, which is fast\n"
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

    printf("===================== ------------- =====================\n");
    printf("===================== Problem size: =====================\n");
    printf("===================== ------------- =====================\n");
    for (int size : sizes) {
        prob_size(mainArgs.numThreads, size);
    }
    printf("========================> Done.\n\n");

    printf("=================---------------------- =================\n");
    printf("================= Problem size (mixed): =================\n");
    printf("=================---------------------- =================\n");
    for (int size : sizes) {
        prob_size_mixed(mainArgs.numThreads, size, mainArgs.p);
    }

    printf("========================> Done.\n\n");
}