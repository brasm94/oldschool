CXX=g++ 
CXXFLAGS = -Wall -g -O2 -fopenmp

SRC_DIR=./src
BUILD_DIR=./build
SRC_FILES =$(wildcard $(SRC_DIR)/*.cpp)
OBJS =$(SRCS:%=$(BUILD_DIR)/%.o)
OBJ_FILES =$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

run:main
	./main

main:$(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY:clean

clean:
	rm build/*.o main
