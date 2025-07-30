LOCAL_LIB_DIR := lib
GLOBAL_LIB_DIR := ../src/lib
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I$(LOCAL_LIB_DIR) -I$(GLOBAL_LIB_DIR) -I$(GLOBAL_LIB_DIR)/bitsery/include
SRCS := $(wildcard *.cpp)
EXES := $(SRCS:.cpp=)
LIBS := $(patsubst %.cpp,%.tool.o,$(wildcard $(LOCAL_LIB_DIR)/*/*.cpp) \
	                         $(wildcard $(LOCAL_LIB_DIR)/*.cpp) \
	                         $(wildcard $(GLOBAL_LIB_DIR)/*/*.cpp))

%.tool.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(EXES): % : %.cpp $(LIBS)
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@ 

clean:
	rm -f $(LIBS) $(EXES)
	
.PHONY: clean