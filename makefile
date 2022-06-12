CXX=g++ -m32 -std=c++20 -msse2
# add -O3, -g for debugging, -ggdb for perf, -fopenmp for openMP
# -m32 for 4 byte pointers
# msse2 enables sse simd support
CXXFLAGS=-Wall -Wno-unknown-pragmas
# add -latomic -mcx16 and -march=native for 16bit atomic loads/stores
# march native will allow the compilier to generate instructions 
# for the system it is compiling on.
LDFLAGS=-fopenmp -latomic

# comment out later
ADD_ARGS=-g

MAINDIR=src
MAIN_SOURCES=$(wildcard $(MAINDIR)/*.cpp)
MAIN_OBJS=$(MAIN_SOURCES:$(MAINDIR)/%.cpp=$(MAINDIR)/%.o)
MAIN=main # executable

TESTDIR=tests
TEST_SOURCES=$(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS=$(TEST_SOURCES:$(TESTDIR)/%.cpp=$(TESTDIR)/%.o)
TEST=test

.PHONY: clean

default: $(MAIN)

# ================= test ======================
$(TEST): ADD_ARGS=-g #-mcx16

# make sure to add linker flags towards the end of the recipe
$(TEST): $(TEST_OBJS) $(TESTDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $(TESTDIR)/$@ $(TEST_OBJS) $(ADD_ARGS) $(TESTDIR)/skiplist.o $(LDFLAGS)

$(TESTDIR)/%.o: $(TESTDIR)/%.cpp tests/test.h helpers/debug.h
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

$(TESTDIR)/skiplist.o: skiplist.cpp skiplist.h $(helpers/*.h)
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

# ================= main ======================
$(MAIN): ADD_ARGS=

$(MAIN): $(MAIN_OBJS) $(MAINDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $(MAINDIR)/$@ $(MAIN_OBJS) $(ADD_ARGS) $(MAINDIR)/skiplist.o $(LDFLAGS)

$(MAINDIR)/%.o: $(MAINDIR)/%.cpp src/problems.h
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

$(MAINDIR)/skiplist.o: skiplist.cpp skiplist.h $(helpers/*.h)
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@


# implicit rule
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $^ -o $@


clean:
	/bin/rm -rf *~ $(TESTDIR)/$(TEST) $(MAINDIR)/$(MAIN) \
	$(TESTDIR)/*.o $(MAINDIR)/*.o