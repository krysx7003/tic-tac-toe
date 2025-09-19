#include "player_manager.h"

std::vector<std::string> PlayerManager::options = {Human, AI_1, AI_2};

std::vector<std::string> PlayerManager::GetOptions() { return options; }

bool PlayerManager::RequestHandled;

Player PlayerManager::player1 = Player("Human", Player::O);
Player PlayerManager::player2 = Player("Human", Player::X);
char PlayerManager::Curr_player = Player::O;
std::string PlayerManager::BoardState = "/ / / / / / / / / /";

void PlayerManager::PrintOptions() {
	for (int i = 0; i < options.size(); i++) {
		printf("%d. %s\n", i, options[i].c_str());
	}
}
std::string PlayerManager::GetOption(int id) { return options[id]; }
bool PlayerManager::ValidOption(char c) {
	if (c > '0' && c < '9')
		return false;

	int id = c - '0';
	if (id >= options.size())
		return false;

	return true;
}

void PlayerManager::StartServer() {}

void PlayerManager::ConnectPlayers() {
	player1.Connect();
	player2.Connect();
}

int PlayerManager::MakeMove() {
	if (Curr_player == Player::O) {
		return player1.Prompt();
	} else {
		return player2.Prompt();
	}
}

bool PlayerManager::CurrPlayerHuman() {
	if (Curr_player == Player::O) {
		return player1.option == options[0];
	} else {
		return player2.option == options[0];
	}
}

void PlayerManager::SetPlayers(std::string playerO, std::string playerX) {
	player1 = Player(playerO, Player::O);
	player2 = Player(playerX, Player::X);
}

void PlayerManager::MsgAll(std::string message) {
	player1.Msg(message);
	player2.Msg(message);
}
