CC = g++
CFLAGS = -ggdb -lasan -lIrrlicht -ltermbox

main: map

map:map.cpp renderer
	$(CC) $(CFLAGS) renderer.o map.cpp -o map 
renderer: renderer3d.cpp
	$(CC) $(CFLAGS) renderer3d.cpp -c -o renderer.o
merge: merge.cpp
	$(CC) $(CFLAGS) merge.cpp -o merge
while: while.cpp
	$(CC) $(CFLAGS) while.cpp -o while
pathfinding: pathfinding.cpp
	$(CC) $(CFLAGS) pathfinding.cpp -o pathfinding
beatenzone: beatenzone.cpp
	$(CC) $(CFLAGS) beatenzone.cpp -o beatenzone
