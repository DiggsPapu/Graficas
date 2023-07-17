all: library mathLibrary

mathLibrary: mathLibrary.cpp
	g++ -o mathLibrary mathLibrary.cpp -lpthread 

library: library.cpp mathLibrary.cpp
	g++ -o library library.cpp mathLibrary.cc -lpthread
