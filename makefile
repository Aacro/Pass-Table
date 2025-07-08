#Name: Arturo Calanche
#Email: aac21p@fsu.edu
#Class: COP4530-0001.fa24
#All work was performed by Arturo Calanche

all: proj5.x

proj5.x: passserver.o proj5.o
	g++ -o proj5.x passserver.o proj5.o -std=c++11 -lcrypt

passserver.o: passserver.cpp passserver.h hashtable.h
	g++ -c passserver.cpp -std=c++11

proj5.o: proj5.cpp passserver.h hashtable.h
	g++ -c proj5.cpp -std=c++11

clean:
	rm -f proj5.x *.o