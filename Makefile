CXX = g++
CXXFLAGS = -Wall -Wsign-compare -g -std=c++11

all: B1_photo_classification B2_shortest_paths B3_bellman_ford

B1_photo_classification: B1_photo_classification.o
	$(CXX) $(CXXFLAGS) B1_photo_classification.o -o B1_photo_classification

B1_photo_classification.o: B1_photo_classification.cpp
	$(CXX) $(CXXFLAGS) -c B1_photo_classification.cpp

B2_shortest_paths: B2_shortest_paths.o
	$(CXX) $(CXXFLAGS) B2_shortest_paths.o -o B2_shortest_paths

B2_shortest_paths.o: B2_shortest_paths.cpp
	$(CXX) $(CXXFLAGS) -c B2_shortest_paths.cpp

B3_bellman_ford: B3_bellman_ford.o
	$(CXX) $(CXXFLAGS) B3_bellman_ford.o -o B3_bellman_ford

B3_bellman_ford.o: B3_bellman_ford.cpp
	$(CXX) $(CXXFLAGS) -c B3_bellman_ford.cpp

test_photo: B1_photo_classification
	./B1_photo_classification

test_paths: B2_shortest_paths
	./B2_shortest_paths

test_bellman: B3_bellman_ford
	./B3_bellman_ford

test: B1_photo_classification B2_shortest_paths B3_bellman_ford
	./B1_photo_classification
	./B2_shortest_paths
	./B3_bellman_ford

run: clean test
	./B1_photo_classification
	./B2_shortest_paths
	./B3_bellman_ford

clean:
	rm -f *.o B1_photo_classification B2_shortest_paths B3_bellman_ford