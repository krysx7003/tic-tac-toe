#include "game.h"
#include "tile.h"

Game::Game(bool gui) {
	curr_player = Player::O;
	board = Board(gui);
	board.setupBuffers();
}

void Game::start() {
	while (active) {
		system("clear");

		printf("Current player: %c", getPlayer());
		char *state = board.getTilesState();
		printf("\n-------------\n");
		printf("| %c | %c | %c | | 0 | 1 | 2 |\n----------\n", state[0], state[1], state[2]);
		printf("| %c | %c | %c | | 3 | 4 | 5 |\n----------\n", state[3], state[4], state[5]);
		printf("| %c | %c | %c | | 6 | 7 | 8 |\n----------\n", state[6], state[7], state[8]);

		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);

		while (!chosenTile(id)) {
			printf("Invalid input. Next move\n> ");
			scanf("%d", &id);
		}
	}
	if (!isDraw()) {
		char *state = board.getTilesState();
		printf("\n-------------\n");
		printf("| %c | %c | %c |\n----------\n", state[0], state[1], state[2]);
		printf("| %c | %c | %c |\n----------\n", state[3], state[4], state[5]);
		printf("| %c | %c | %c |\n----------\n", state[6], state[7], state[8]);

		printf("\nPlayer %c won\n", getWinner());
	} else {
		printf("\nGame ended with a draw\n");
	}
}

void Game::restart() {
	curr_player = Player::O;
	lastTile = -1;
	winner = -1;
	active = true;

	board.setTilesState();
}

bool Game::chosenTile(float x, float y) {
	if (!active) {
		return false;
	}

	int col = board.getTileCol(x);
	int row = board.getTileRow(y) * 3;
	int tileId = col + row;

	if (board.takeTile(tileId, curr_player)) {
		swapPlayer();
		lastTile = tileId;
		return true;
	}
	return false;
}

bool Game::chosenTile(int tileId) {
	if (!active) {
		return false;
	}

	if (board.takeTile(tileId, curr_player)) {
		swapPlayer();
		lastTile = tileId;
		return true;
	}
	return false;
}

void Game::swapPlayer() {
	if (!active) {
		return;
	}
	if (curr_player == Player::O) {
		curr_player = Player::X;
	} else {
		curr_player = Player::O;
	}

	if (winCondition()) {
		active = false;
	} else if (isDraw()) {
		active = false;
	}
}

void Game::setWinner(char state) {
	if (state == TileState::TakenO) {
		winner = Player::O;
	} else if (state == TileState::TakenX) {
		winner = Player::X;
	}
}

bool Game::winCondition() {
	for (int i = 0; i < BOARD_WIDTH; i++) {
		if (horizontalLine(i))
			return true;
		if (verticalLine(i))
			return true;
	}

	if (diagonalLeftLine())
		return true;

	if (diagonalRightLine())
		return true;
	return false;
}

bool Game::verticalLine(int col) {
	char first = board.tiles_state[0 + col];
	if (first == TileState::Empty)
		return false;

	for (int row = 1; row < BOARD_WIDTH; row++) {
		if (board.tiles_state[col + row * 3] != first)
			return false;
	}

	setWinner(first);
	return true;
}

bool Game::horizontalLine(int row) {
	char first = board.tiles_state[0 + row * 3];
	if (first == TileState::Empty)
		return false;

	for (int col = 1; col < BOARD_WIDTH; col++) {
		if (board.tiles_state[col + row * 3] != first)
			return false;
	}

	setWinner(first);
	return true;
}

bool Game::diagonalLeftLine() {
	char topLeft = board.tiles_state[0]; // TOOD - Replace with enum
	if (topLeft != TileState::Empty) {
		if (board.tiles_state[4] == topLeft && board.tiles_state[8] == topLeft) {
			setWinner(topLeft);
			return true;
		}
	}
	return false;
}
bool Game::diagonalRightLine() {
	char topRight = board.tiles_state[2]; // TOOD - Replace with enum
	if (topRight != TileState::Empty) {
		if (board.tiles_state[4] == topRight && board.tiles_state[6] == topRight) {
			setWinner(topRight);
			return true;
		}
	}
	return false;
}

bool Game::isDraw() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (board.tiles_state[i] == TileState::Empty) {
			return false;
		}
	}
	winner = 0;
	return true;
}

char Game::getPlayer() { return curr_player; }

char Game::getWinner() { return winner; }

short Game::getLastTile() { return lastTile; }
