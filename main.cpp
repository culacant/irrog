#include "game.h"

int main()
{
	Game *game = new Game();
	game->Run();
	delete game;
	return 1;
}
