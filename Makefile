CXX=g++
CXXFLAGS=-std=c++11 `Magick++-config --cppflags --cxxflags`
LDFLAGS=`Magick++-config --ldflags`
LIBS=-lVTFLib13 -lgflags `Magick++-config --libs`

.PHONY: all
all: vtfconv

obj/formatnames.o: formatnames.cpp
	$(CXX) $(CXXFLAGS) -c formatnames.cpp -o obj/formatnames.o

obj/vtfconv.o: vtfconv.cpp
	$(CXX) $(CXXFLAGS) -c vtfconv.cpp -o obj/vtfconv.o

vtfconv: obj/vtfconv.o obj/formatnames.o
	$(CXX) $(LDFLAGS) obj/vtfconv.o obj/formatnames.o $(LIBS) -o vtfconv

.PHONY: clean
clean:
	rm -f obj/*.o
	rm -f vtfconv

