#include "core/resource_manager.h"

#include "game.h"
#include "player_manager.h"
#include "tile.h"
#include <glm/ext/vector_float2.hpp>

void Game::Init() {
	config = ResourceManager::Config;
	top_menu_height = config["top_menu"]["height"].get<int>();

	bool gui = config["gui"];

	if (gui) {
		ResourceManager::LoadShader("piece.vs", "piece.frag", "piece");
		ResourceManager::LoadShader("flat_argb.vs", "flat_argb.frag", "argb");

		ResourceManager::LoadTexture("X.png", true, "X");
		ResourceManager::LoadTexture("O.png", true, "O");

		ResourceManager::LoadTexture("h_line.png", true, "horizontal");
		ResourceManager::LoadTexture("v_line.png", true, "vertical");
		ResourceManager::LoadTexture("d_r_line.png", true, "diagonal_r");
		ResourceManager::LoadTexture("d_l_line.png", true, "diagonal_l");
	}

	board.Init();
}

void Game::Start(std::string player1, std::string player2) {
	PlayerManager::Curr_player = Player::O;
	PlayerManager::SetPlayers(player1, player2);

	PlayerManager::StartServer();

	active = true;
}

void Game::Print() {
	std::vector<char> state = board.GetTilesState();
	while (active) {
		system("clear");

		board.Print(true);
		printf("Current player: %c\n", PlayerManager::Curr_player);
		int id = -1;
		do {
			PlayerManager::BoardState = board.GetState();
			id = PlayerManager::MakeMove();
		} while (!ChosenTile(id));
	}

	if (!IsDraw(state)) {
		board.Print(false);

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
	PlayerManager::Curr_player = Player::O;
	lastTile = -1;
	winner = -1;
	ended = false;
	active = false;

	board.RestetTiles();
}

bool Game::ChosenTile(double x, double y) {
	if (!active) {
		return false;
	}

	int tileId = board.TileUnderMouse(x, y);

	if (board.TakeTile(tileId)) {
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

	if (board.TakeTile(tileId)) {
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
	if (PlayerManager::Curr_player == Player::O) {
		PlayerManager::Curr_player = Player::X;
	} else {
		PlayerManager::Curr_player = Player::O;
	}

	std::vector<char> state = board.GetTilesState();
	if (WinCondition(state)) {
		ended = true;
		active = false;
	} else if (IsDraw(state)) {
		ended = true;
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

char Game::GetWinner() { return winner; }

short Game::GetLastTile() { return lastTile; }
