
#include "../header/Game.h"

int main() {
	Game game(true);
	game.Run();
	return 0;
}

int WinMain() {
	Game game(false);
	game.Run();
	return 0;
}