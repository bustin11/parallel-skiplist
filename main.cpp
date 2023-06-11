
// #include "problems.h"
// #include "src/skiplist.h"
#include"skiplist.h"
#include"problems.h"

#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <string>
#include <vector>

struct mainArgs_t {
    int help;
    int numThreads;
    float p;
    std::string fileName;
} mainArgs;


void print_usage () {
    puts("Usage: ./main [-h]"
        "Options:\n"
        "  -h           Print this help message.\n"
        "  -p <prob>    Probability of Insertion\n"
        "  -f <file>    Output file results\n"
        "DESCRIPTION:\n"
        "The higher the probability, the slower it runs since we could try\n"
        "removing elements that doesn't exist, which is fast\n"
        );
}


int main(int argc, char** argv) {

    int opt;
    mainArgs.numThreads = 1;
    mainArgs.p = .5;
    mainArgs.fileName = "";
    while ((opt = getopt(argc, argv, "hp:f:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'p':
                mainArgs.p = atof(optarg);
                break;
            case 'f':
                mainArgs.fileName = optarg;
        }
    }

    std::vector<int> sizes;
    sizes.push_back(10000);
    sizes.push_back(20000);
    sizes.push_back(30000);
    printf("Number of threads:%d\n", mainArgs.numThreads);

    printf("===================== ------------- =====================\n");
    printf("===================== Problem size: =====================\n");
    printf("===================== ------------- =====================\n");
    for (int size : sizes) {
        prob_size(mainArgs.numThreads, size, mainArgs.fileName);
    }
    printf("========================> Done.\n\n");

    printf("=================---------------------- =================\n");
    printf("================= Problem size (mixed): =================\n");
    printf("=================---------------------- =================\n");
    for (int size : sizes) {
        prob_size_mixed(mainArgs.numThreads, size, mainArgs.p, mainArgs.fileName);
    }

    printf("========================> Done.\n\n");
}