#include "utils/resource_manager.h"

#include "game.h"
#include "tile.h"
#include <glm/ext/vector_float2.hpp>

void Game::Init() {
	config = ResourceManager::Config;
	top_menu_height = config["top_menu"]["height"].get<int>();

	curr_player = Player::O;

	ResourceManager::LoadShader("line.vs", "line.frag", "line");
	ResourceManager::LoadShader("piece.vs", "piece.frag", "piece");
	ResourceManager::LoadShader("tile.vs", "tile.frag", "tile");

	ResourceManager::LoadTexture("X.png", true, "X");
	ResourceManager::LoadTexture("O.png", true, "O");

	ResourceManager::LoadTexture("h_line.png", true, "horizontal");
	ResourceManager::LoadTexture("v_line.png", true, "vertical");
	ResourceManager::LoadTexture("d_r_line.png", true, "diagonal_r");
	ResourceManager::LoadTexture("d_l_line.png", true, "diagonal_l");

	board.Init();
	board.SetupBuffers();
}

void Game::Start() {
	while (active) {
		system("clear");

		printf("Current player: %c", GetPlayer());
		char *state = board.GetTilesState();
		printf("\n-------------\n");
		printf("| %c | %c | %c | | 0 | 1 | 2 |\n----------\n", state[0], state[1], state[2]);
		printf("| %c | %c | %c | | 3 | 4 | 5 |\n----------\n", state[3], state[4], state[5]);
		printf("| %c | %c | %c | | 6 | 7 | 8 |\n----------\n", state[6], state[7], state[8]);

		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);

		while (!ChosenTile(id)) {
			printf("Invalid input. Next move\n> ");
			scanf("%d", &id);
		}
	}
	if (!IsDraw()) {
		char *state = board.GetTilesState();
		printf("\n-------------\n");
		printf("| %c | %c | %c |\n----------\n", state[0], state[1], state[2]);
		printf("| %c | %c | %c |\n----------\n", state[3], state[4], state[5]);
		printf("| %c | %c | %c |\n----------\n", state[6], state[7], state[8]);

		printf("\nPlayer %c won\n", GetWinner());
	} else {
		printf("\nGame ended with a draw\n");
	}
}

void Game::Render() {
	board.Render();

	if (!active) {
		board.RenderWin(ResourceManager::GetTexture(texture), texture_pos, texture_size);
	}
}

void Game::Restart() {
	curr_player = Player::O;
	lastTile = -1;
	winner = -1;
	active = true;

	board.SetTilesState();
}

bool Game::ChosenTile(float x, float y) {
	if (!active) {
		return false;
	}

	int col = board.GetTileCol(x);
	int row = board.GetTileRow(y) * 3;
	int tileId = col + row;

	if (board.TakeTile(tileId, curr_player)) {
		swapPlayer();
		lastTile = tileId;
		return true;
	}
	return false;
}

bool Game::ChosenTile(int tileId) {
	if (!active) {
		return false;
	}

	if (board.TakeTile(tileId, curr_player)) {
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

	if (WinCondition()) {
		active = false;
	} else if (IsDraw()) {
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

bool Game::WinCondition() {
	for (int i = 0; i < board.width; i++) {
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

	for (int row = 1; row < board.width; row++) {
		if (board.tiles_state[col + row * 3] != first)
			return false;
	}

	setWinner(first);

	texture = "vertical";
	texture_pos = glm::vec2(col * 200, top_menu_height);
	texture_size = glm::vec2(200, 600);

	return true;
}

bool Game::horizontalLine(int row) {
	char first = board.tiles_state[0 + row * 3];
	if (first == TileState::Empty)
		return false;

	for (int col = 1; col < board.width; col++) {
		if (board.tiles_state[col + row * 3] != first)
			return false;
	}

	setWinner(first);

	texture = "horizontal";
	texture_pos = glm::vec2(0, row * 200 + top_menu_height);
	texture_size = glm::vec2(600, 200);

	return true;
}

bool Game::diagonalLeftLine() {
	char topLeft = board.tiles_state[0]; // TOOD - Replace with enum
	if (topLeft != TileState::Empty) {
		if (board.tiles_state[4] == topLeft && board.tiles_state[8] == topLeft) {
			setWinner(topLeft);
			texture = "diagonal_l";
			texture_pos = glm::vec2(0, top_menu_height);
			texture_size = glm::vec2(600, 600);

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
			texture = "diagonal_r";
			texture_pos = glm::vec2(0, top_menu_height);
			texture_size = glm::vec2(600, 600);

			return true;
		}
	}
	return false;
}

bool Game::IsDraw() {
	for (int i = 0; i < board.tiles_num; i++) {
		if (board.tiles_state[i] == TileState::Empty) {
			return false;
		}
	}
	winner = 0;
	return true;
}

char Game::GetPlayer() { return curr_player; }

char Game::GetWinner() { return winner; }

short Game::GetLastTile() { return lastTile; }
