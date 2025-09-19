#include "player_manager.h"

std::vector<std::string> PlayerManager::options = {"Human", "Ai 1", "Ai 2"};

std::vector<std::string> PlayerManager::GetOptions() { return options; }

std::string PlayerManager::GetOption(int id) { return options[id]; }

bool PlayerManager::RequestHandled;

Player PlayerManager::player1 = Player("Human", Player::O);
Player PlayerManager::player2 = Player("Human", Player::X);
char PlayerManager::Curr_player = Player::O;

void PlayerManager::PrintOptions() {
	for (int i = 0; i < options.size(); i++) {
		printf("%d. %s\n", i, options[i].c_str());
	}
}
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
