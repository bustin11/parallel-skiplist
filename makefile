CXX=g++ -m64 -std=c++20
# add -O3 and remove -g for non-debugging
CXXFLAGS=-g -Wall -Wno-unknown-pragmas
LDFLAGS=
SOURCES=main.cpp skiplist.cpp test.cpp
OBJDIR=objs
OBJS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)
EXECUTABLE=main

.PHONY: dir clean


default: $(EXECUTABLE)

dirs:
	mkdir -p $(OBJDIR)/

$(EXECUTABLE): dirs $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(SOURCES) : skiplist.h

# implicit rule
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $^ -o $@




clean:
	/bin/rm -rf *~ $(OBJDIR) $(EXECUTABLE) *.class