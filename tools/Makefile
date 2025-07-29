CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Ilib
SRCS := $(wildcard *.cpp)
EXES := $(SRCS:.cpp=)
LIBS := $(patsubst %.cpp,%.o,$(wildcard lib/*/*.cpp) $(wildcard lib/*.cpp))

lib/%.o: lib/%.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(EXES): % : %.cpp $(LIBS)
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ 

clean:
	rm -f lib/*.o $(EXES)
	
.PHONY: clean