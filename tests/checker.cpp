
#include "test.h"
#include "../skiplist.h"

#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <string>
#include <vector>
#include <map>

struct mainArgs_t {
    int help;
    int testID;
    int numThreads;
    int seed;
} mainArgs;


void print_usage () {
    puts("Usage: ./main [-h] -T <numThreads> -n <testNumber>\n"
        "Options:\n"
        "  -h         Print this help message.\n"
        "  -N <Threads> Number of threads to launch\n"
        "  -s <seed>  seed for RNG (only applies to test 5,6,11,12)\n"
        "  -T <num>   Test Number (1-12)\n"
        "    - 1-6 test insertions, with increasing difficulty to pass\n"
        "    - 7-12 test deletions, with increasing difficulty to pass\n"
        "    - 13 test 100 ins/del mixed (don't try unless 1-12 passed)\n"
        "    - 14 test 10000 ins/del mixed (don't try unless 13 passed)\n"
        );
}

typedef bool (*easyTestPtr)();
typedef bool (*hardTestPtr)(int, int);

void initTests(std::vector<easyTestPtr>& easyTests, 
                std::vector<hardTestPtr>& hardTests,
                    std::map<int, int>& index2Test) {
    easyTests.push_back(test1);
    easyTests.push_back(test2);
    easyTests.push_back(test3);
    easyTests.push_back(test4);
    index2Test[1] = 0;
    index2Test[2] = 1;
    index2Test[3] = 2;
    index2Test[4] = 3;

    hardTests.push_back(test5);
    hardTests.push_back(test6);
    index2Test[5] = 0;
    index2Test[6] = 1;

    easyTests.push_back(test7);
    easyTests.push_back(test8);
    easyTests.push_back(test9);
    easyTests.push_back(test10);
    index2Test[7] = 4;
    index2Test[8] = 5;
    index2Test[9] = 6;
    index2Test[10] = 7;

    hardTests.push_back(test11);
    hardTests.push_back(test12);
    index2Test[11] = 2;
    index2Test[12] = 3;

    hardTests.push_back(test13);
    hardTests.push_back(test14);
    index2Test[13] = 4;
    index2Test[14] = 5;
}



int main(int argc, char** argv) {

    int opt;
    mainArgs.testID = 0;
    mainArgs.seed = -1;
    mainArgs.numThreads = 1;
    while ((opt = getopt(argc, argv, "hT:s:N:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                return 0;
            case 'T':
                mainArgs.testID = atoi(optarg);
                break;
            case 's':
                mainArgs.seed = atoi(optarg);
                break;
            case 'N':
                mainArgs.numThreads = atoi(optarg);
                break;
        }
    }

    std::vector<easyTestPtr> easyTests;
    std::vector<hardTestPtr> hardTests;
    std::map<int, int> index2test;
    initTests(easyTests, hardTests, index2test);
    size_t numTests = easyTests.size() + hardTests.size();

    printf("Number of threads:%d\n", mainArgs.numThreads);
    for (int i=1; i<=static_cast<int>(numTests); i++) {
        if (i == mainArgs.testID || !mainArgs.testID) {
            if (i == 5 || i == 6 || i >= 11) { 
                assert(hardTests[index2test[i]](mainArgs.numThreads, 
                                        mainArgs.seed));
            } else {
                assert(easyTests[index2test[i]]());
            }
        }
    }

    printf("All Tests Passed!\n");
}
