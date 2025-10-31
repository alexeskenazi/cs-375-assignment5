CXX = g++
CXXFLAGS = -Wall -Wsign-compare -g -std=c++11

all: photo_classification shortest_paths

photo_classification: photo_classification.o
	$(CXX) $(CXXFLAGS) photo_classification.o -o photo_classification

photo_classification.o: photo_classification.cpp
	$(CXX) $(CXXFLAGS) -c photo_classification.cpp

shortest_paths: shortest_paths.o
	$(CXX) $(CXXFLAGS) shortest_paths.o -o shortest_paths

shortest_paths.o: shortest_paths.cpp
	$(CXX) $(CXXFLAGS) -c shortest_paths.cpp

test_photo: photo_classification
	./photo_classification

test_paths: shortest_paths
	./shortest_paths

test: photo_classification shortest_paths
	./photo_classification
	./shortest_paths

run: clean test
	./photo_classification
	./shortest_paths

clean:
	rm -f *.o photo_classification shortest_paths