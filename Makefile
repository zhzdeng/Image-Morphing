FLAG = -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
OBJECT = main.o Morphing.o delaunay.o
PROJECT = morphing
CC = g++
goal: ${OBJECT}
	${CC} ${OBJECT} ${FLAG} -o ${PROJECT}
main.o: Morphing.hpp main.cpp
	${CC} -c main.cpp ${FLAG}
Morphing.o: Morphing.hpp delaunay.hpp CImg.h Morphing.cpp
	${CC} -c Morphing.cpp ${FLAG}
delaunay.o: delaunay.hpp delaunay.cpp
	${CC} -c delaunay.cpp
clear:
	-rm ${OBJECT}
	-rm ${PROJECT}
