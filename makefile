CC = g++
CFLAGS = -lasan -lIrrlicht -ggdb

irrog: main.o
	$(CC) $(CFLAGS) obj/pathfinder.o obj/input.o obj/game.o obj/renderer.o obj/main.o obj/map.o obj/squad.o obj/actor.o obj/weapon.o -o irrog
main.o: main.cpp game.o
	$(CC) $(CFLAGS) main.cpp -c -o obj/main.o
game.o: game.cpp renderer.o map.o squad.o actor.o weapon.o input.o pathfinder.o
	$(CC) $(CFLAGS) game.cpp -c -o obj/game.o
renderer.o: renderer.cpp map.o actor.o
	$(CC) $(CFLAGS) renderer.cpp -c -o obj/renderer.o
map.o: map.cpp
	$(CC) $(CFLAGS) map.cpp -c -o obj/map.o
squad.o: squad.cpp
	$(CC) $(CFLAGS) squad.cpp -c -o obj/squad.o
actor.o: actor.cpp
	$(CC) $(CFLAGS) actor.cpp -c -o obj/actor.o
weapon.o: weapon.cpp
	$(CC) $(CFLAGS) weapon.cpp -c -o obj/weapon.o
input.o: input.cpp
	$(CC) $(CFLAGS) input.cpp -c -o obj/input.o
pathfinder.o: pathfinder.cpp
	$(CC) $(CFLAGS) pathfinder.cpp -c -o obj/pathfinder.o
clean:
	rm obj/* irrog
