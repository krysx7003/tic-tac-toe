#include "player.h"

std::vector<std::string> Player::options = {"Human", "Ai 1", "Ai 2"};

std::vector<std::string> Player::GetOptions() { return options; }

std::string Player::GetOption(int id) { return options[id]; }

void Player::PrintOptions() {
	for (int i = 0; i < options.size(); i++) {
		printf("%d. %s\n", i, options[i].c_str());
	}
}
bool Player::ValidOption(char c) {
	if (c > '0' && c < '9')
		return false;

	int id = c - '0';
	if (id >= options.size())
		return false;

	return true;
}

int Player::promptPlayer(std::string player) {
	if (player == options[0]) {
		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);
		return id;
	} else if (player == options[1]) {
		printf("ERROR::PLAYER: Input method for this player: %s is not implemented\n",
			   player.c_str());
		// TODO - Implement input
		return -1;

	} else if (player == options[2]) {
		printf("ERROR::PLAYER: Input method for this player: %s is not implemented\n",
			   player.c_str());
		// TODO - Implement input
		return -1;
	} else {
		printf("ERROR::PLAYER: Invalid player name: %s \n", player.c_str());
		exit(-1);
	}
}

int Player::MakeMove(char player) {
	if (player == Player::O) {
		return promptPlayer(player1);
	} else {
		return promptPlayer(player2);
	}
}
