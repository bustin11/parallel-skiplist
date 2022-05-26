CXX=g++ -m64 -std=c++20
# add -O3, -g for debugging, -ggdb for perf, -fopenmp for openMP
CXXFLAGS=-Wall -Wno-unknown-pragmas
LDFLAGS=
TESTDIR=tests
OBJDIR=objs

TEST_SOURCES=$(TESTDIR)/checker.cpp $(TESTDIR)/test.cpp
TEST_OBJS=$(TEST_SOURCES:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)

# EXECUTABLE=run

.PHONY: dir clean


default: $(EXECUTABLE)

dirs:
	mkdir -p $(OBJDIR)/

test: dirs $(TEST_OBJS) $(OBJDIR)/skiplist.o
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS) $(OBJDIR)/skiplist.o

# $(EXECUTABLE): dirs $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR)/skiplist.o: skiplist.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@


$(TEST_SOURCES) : skiplist.h tests/test.h

skiplist.cpp : skiplist.h



# implicit rule
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $^ -o $@




clean:
	/bin/rm -rf *~ $(OBJDIR) $(EXECUTABLE) *.class