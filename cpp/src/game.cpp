#include "utils/resource_manager.h"

#include "game.h"
#include "tile.h"
#include <glm/ext/vector_float2.hpp>

void Game::Init() {
	config = ResourceManager::Config;
	top_menu_height = config["top_menu"]["height"].get<int>();

	curr_player = Player::O;

	ResourceManager::LoadShader("piece.vs", "piece.frag", "piece");
	ResourceManager::LoadShader("flat_argb.vs", "flat_argb.frag", "argb");

	ResourceManager::LoadTexture("X.png", true, "X");
	ResourceManager::LoadTexture("O.png", true, "O");

	ResourceManager::LoadTexture("h_line.png", true, "horizontal");
	ResourceManager::LoadTexture("v_line.png", true, "vertical");
	ResourceManager::LoadTexture("d_r_line.png", true, "diagonal_r");
	ResourceManager::LoadTexture("d_l_line.png", true, "diagonal_l");

	board.Init();
}

void Game::Start() {
	std::vector<char> state = board.GetTilesState();
	while (active) {
		system("clear");

		printf("Current player: %c", GetPlayer());
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
	if (!IsDraw(state)) {
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

	board.RestetTiles();
}

bool Game::ChosenTile(double x, double y) {
	if (!active) {
		return false;
	}

	int tileId = board.TileUnderMouse(x, y);

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

	std::vector<char> state = board.GetTilesState();
	if (WinCondition(state)) {
		active = false;
	} else if (IsDraw(state)) {
		active = false;
	}
}

void Game::setWinner(char state) {
	if (state == Tile::State::TakenO) {
		winner = Player::O;
	} else if (state == Tile::State::TakenX) {
		winner = Player::X;
	}
}

bool Game::WinCondition(std::vector<char> state) {
	for (int i = 0; i < board.width; i++) {
		if (horizontalLine(i, state))
			return true;
		if (verticalLine(i, state))
			return true;
	}

	if (diagonalLeftLine(state))
		return true;

	if (diagonalRightLine(state))
		return true;

	return false;
}

bool Game::verticalLine(int col, std::vector<char> state) {
	char first = state[0 + col];
	if (first == Tile::State::Empty)
		return false;

	for (int row = 1; row < board.width; row++) {
		if (state[col + row * 3] != first)
			return false;
	}

	setWinner(first);

	texture = "vertical";
	texture_pos = glm::vec2(col * 200, top_menu_height);
	texture_size = glm::vec2(200, 600);

	return true;
}

bool Game::horizontalLine(int row, std::vector<char> state) {
	char first = state[0 + row * 3];
	if (first == Tile::State::Empty)
		return false;

	for (int col = 1; col < board.width; col++) {
		if (state[col + row * 3] != first)
			return false;
	}

	setWinner(first);

	texture = "horizontal";
	texture_pos = glm::vec2(0, row * 200 + top_menu_height);
	texture_size = glm::vec2(600, 200);

	return true;
}

bool Game::diagonalLeftLine(std::vector<char> state) {
	char topLeft = state[0]; // TOOD - Replace with enum
	if (topLeft != Tile::State::Empty) {
		if (state[4] == topLeft && state[8] == topLeft) {
			setWinner(topLeft);
			texture = "diagonal_l";
			texture_pos = glm::vec2(0, top_menu_height);
			texture_size = glm::vec2(600, 600);

			return true;
		}
	}
	return false;
}
bool Game::diagonalRightLine(std::vector<char> state) {
	char topRight = state[2]; // TOOD - Replace with enum
	if (topRight != Tile::State::Empty) {
		if (state[4] == topRight && state[6] == topRight) {
			setWinner(topRight);
			texture = "diagonal_r";
			texture_pos = glm::vec2(0, top_menu_height);
			texture_size = glm::vec2(600, 600);

			return true;
		}
	}
	return false;
}

bool Game::IsDraw(std::vector<char> state) {
	for (int i = 0; i < board.tiles_num; i++) {
		if (state[i] == Tile::State::Empty) {
			return false;
		}
	}
	winner = '-';
	return true;
}

char Game::GetPlayer() { return curr_player; }

char Game::GetWinner() { return winner; }

short Game::GetLastTile() { return lastTile; }
