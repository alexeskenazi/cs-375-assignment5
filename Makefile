CXX = g++
CXXFLAGS = -Wall -Wsign-compare -g -std=c++11

all: photo_classification shortest_paths bellman_ford

photo_classification: photo_classification.o
	$(CXX) $(CXXFLAGS) photo_classification.o -o photo_classification

photo_classification.o: photo_classification.cpp
	$(CXX) $(CXXFLAGS) -c photo_classification.cpp

shortest_paths: shortest_paths.o
	$(CXX) $(CXXFLAGS) shortest_paths.o -o shortest_paths

shortest_paths.o: shortest_paths.cpp
	$(CXX) $(CXXFLAGS) -c shortest_paths.cpp

bellman_ford: bellman_ford.o
	$(CXX) $(CXXFLAGS) bellman_ford.o -o bellman_ford

bellman_ford.o: bellman_ford.cpp
	$(CXX) $(CXXFLAGS) -c bellman_ford.cpp

test_photo: photo_classification
	./photo_classification

test_paths: shortest_paths
	./shortest_paths

test_bellman: bellman_ford
	./bellman_ford

test: photo_classification shortest_paths bellman_ford
	./photo_classification
	./shortest_paths
	./bellman_ford

run: clean test
	./photo_classification
	./shortest_paths
	./bellman_ford

clean:
	rm -f *.o photo_classification shortest_paths bellman_ford