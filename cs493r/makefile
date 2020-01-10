CXX=g++ 
CXXFLAGS = -g -O2 -std=gnu++17 -static 
run: running
	./running
running: main.o
	$(CXX) $(CXXFLAGS) -o running main.o
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
