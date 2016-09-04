CXX=g++
CXXFLAGS=-std=c++11 `Magick++-config --cppflags --cxxflags`
LDFLAGS=`Magick++-config --ldflags`
LIBS=-lVTFLib13 `Magick++-config --libs`

.PHONY: all
all: vtfconv

formatnames.o: formatnames.cpp
	$(CXX) $(CXXFLAGS) -c formatnames.cpp

vtfconv.o: vtfconv.cpp
	$(CXX) $(CXXFLAGS) -c vtfconv.cpp

vtfconv: vtfconv.o formatnames.o
	$(CXX) $(LDFLAGS) vtfconv.o formatnames.o $(LIBS) -o vtfconv

.PHONY: clean
clean:
	rm -rf *.o
	rm -f vtfconv

