CC = mpic++

main: mpiVars.o utils.o vector2d.o grid.o solve.o task.o
	$(CC) -w main.cpp utils.o mpiVars.o grid.o solve.o vector2d.o task.o -o main

mpiVars.o:
	$(CC) -c mpiVars.cpp

utils.o:
	$(CC) -c utils.cpp

grid.o:
	$(CC) -c grid.cpp

vector2d.o:
	$(CC) -c vector2d.cpp

solve.o:
	$(CC) -c solve.cpp

task.o:
	$(CC) -c task.cpp

clean:
	rm utils.o mpiVars.o main mpiVars.h.gch solve.o grid.o vector2d.o task.o
