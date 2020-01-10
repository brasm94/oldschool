CXX=g++

# SRC_DIR=./src
# BUILD_DIR=./build
# SRC_FILES =$(wildcard $(SRC_DIR)/*.cpp)
# OBJS =$(SRCS:%=$(BUILD_DIR)/%.o)
# OBJ_FILES =$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

CFLAGS = $(shell pkg-config --cflags glfw3 glew)
# calling program "pkg-config" and store result in LDFLAGS variable
LDFLAGS = $(shell pkg-config --libs glfw3 glew)
LDFLAGS +="-lGL"
# echo $(LDFLAGS)
run:main
	./main
main:main.cpp
#	echo "this" $(CXX) $(CFLAGS) -o main main.cpp $(LDFLAGS)
	$(CXX) $(CFLAGS) -o main main.cpp tank.cpp  particles.h $(LDFLAGS)
# tank:tank.cpp
# 	$(CXX) $(CFLAGS) tank.cpp tank.h $(LDFLAGS)
