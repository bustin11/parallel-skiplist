CXX=g++ -m64 -std=c++20
# add -O3, -g for debugging, -ggdb for perf, -fopenmp for openMP
CXXFLAGS=-Wall -Wno-unknown-pragmas
LDFLAGS=
OBJDIR=objs

# comment out later
ADD_ARGS=-g
MAIN_SOURCES=main.cpp problems.cpp
MAIN_OBJS=$(MAIN_SOURCES:%.cpp=$(OBJDIR)/%.o)
MAIN=main # executable

TESTDIR=tests
TEST_SOURCES=$(TESTDIR)/checker.cpp $(TESTDIR)/test.cpp
TEST_OBJS=$(TEST_SOURCES:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)
TEST=test

.PHONY: dir clean


default: $(MAIN)

dirs:
	mkdir -p $(OBJDIR)/

$(TEST): ADD_ARGS=-g

$(MAIN): ADD_ARGS=-fopenmp


$(TEST): dirs $(TEST_OBJS) $(OBJDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS) $(ADD_ARGS) $(OBJDIR)/skiplist.o

$(MAIN): dirs $(MAIN_OBJS) $(OBJDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJS) $(ADD_ARGS) $(OBJDIR)/skiplist.o


$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

$(OBJDIR)/%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@

$(OBJDIR)/skiplist.o: skiplist.cpp
	$(CXX) $< $(CXXFLAGS) $(ADD_ARGS) -c -o $@


$(TEST_SOURCES) : skiplist.h tests/test.h

$(MAIN_SOURCES) : skiplist.h problems.h

skiplist.cpp : skiplist.h



# implicit rule
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $^ -o $@




clean:
	/bin/rm -rf *~ $(OBJDIR) test *.class main