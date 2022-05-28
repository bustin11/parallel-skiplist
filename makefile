CXX=g++ -m64 -std=c++20
# add -O3, -g for debugging, -ggdb for perf, -fopenmp for openMP
CXXFLAGS=-Wall -Wno-unknown-pragmas
LDFLAGS=

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
$(TEST): ADD_ARGS=-g -D"OMP=0"

$(TEST): $(TEST_OBJS) $(TESTDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $(TESTDIR)/$@ $(TEST_OBJS) $(ADD_ARGS) $(TESTDIR)/skiplist.o

$(TESTDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

$(TESTDIR)/skiplist.o: skiplist.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

# ================= main ======================
$(MAIN): ADD_ARGS=-fopenmp -D"OMP=1"

$(MAIN): $(MAIN_OBJS) $(MAINDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $(MAINDIR)/$@ $(MAIN_OBJS) $(ADD_ARGS) $(MAINDIR)/skiplist.o

$(MAINDIR)/%.o: $(MAINDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

$(MAINDIR)/skiplist.o: skiplist.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

# ================= headers ======================
$(TEST) : skiplist.h tests/test.h helpers/debug.h

$(MAIN) : skiplist.h src/problems.h

skiplist.cpp : skiplist.h helpers/debug.h



# implicit rule
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $^ -o $@




clean:
	/bin/rm -rf *~ $(TESTDIR)/$(TEST) $(MAINDIR)/$(MAIN) \
	$(TESTDIR)/*.o $(MAINDIR)/*.o