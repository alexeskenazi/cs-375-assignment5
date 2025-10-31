CXX = g++
CXXFLAGS = -Wall -Wsign-compare -g -std=c++11

all: photo_classification

photo_classification: photo_classification.o
	$(CXX) $(CXXFLAGS) photo_classification.o -o photo_classification

photo_classification.o: photo_classification.cpp
	$(CXX) $(CXXFLAGS) -c photo_classification.cpp

test_photo: photo_classification
	./photo_classification

test: photo_classification
	./photo_classification

run: clean test
	./photo_classification

clean:
	rm -f *.o photo_classification