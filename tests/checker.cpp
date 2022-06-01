
#include "test.h"
#include "../skiplist.h"
#include "../helpers/debug.h"

#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <string>
#include <vector>

struct mainArgs_t {
    int help;
    int testID;
} mainArgs;


void print_usage () {
    puts("Usage: ./main [-h] -T <numThreads> -n <testNumber>\n"
        "Options:\n"
        "  -h         Print this help message.\n"
        "  -T <num>   Test Number (1-12)\n"
        "    - 1-6 test insertions, with increasing difficulty to pass\n"
        "    - 7-12 test removal, with increasing difficulty to pass\n"
        );
}

typedef bool (*fnPtr)();

void initTests(std::vector<fnPtr>& tests) {
    tests.push_back(test1);
    tests.push_back(test2);
    tests.push_back(test3);
    tests.push_back(test4);
    tests.push_back(test5);
    tests.push_back(test6);
    tests.push_back(test7);
    tests.push_back(test8);
    tests.push_back(test9);
    tests.push_back(test10);
    tests.push_back(test11);
    tests.push_back(test12);
}



int main(int argc, char** argv) {

    int opt;
    mainArgs.testID = 0;
    while ((opt = getopt(argc, argv, "hT:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'T':
                mainArgs.testID = atoi(optarg);
                break;
        }
    }

    std::vector<fnPtr> tests;
    initTests(tests);

    if (DEBUG) srand(11);
    else srand(time(NULL));

    for (int i=1; i<=(int)tests.size(); i++) {
        if (i == mainArgs.testID || !mainArgs.testID) {
            assert(tests[i-1]());
        }
    }

    printf("All Tests Passed!\n");
}