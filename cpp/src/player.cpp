#include "player.h"
#include "player_manager.h"

int Player::makeRequest() {
	int id = -1;

	// TODO - Implement input
	printf("Move:%s", PlayerManager::BoardState.c_str());
	scanf(" %d", &id);
	return id;
}

int Player::Prompt() {
	if (option == PlayerManager::Human) {
		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);
		return id;

	} else if (option == PlayerManager::AI_1) {
		return this->makeRequest();

	} else if (option == PlayerManager::AI_2) {
		return this->makeRequest();

	} else {
		printf("ERROR::PLAYER: Invalid option name: %s \n", option.c_str());
		exit(-1);
	}
}

void Player::Connect() {}

void Player::Msg(std::string message) {
	if (option == PlayerManager::AI_1) {
		// TODO - Implement output
		printf("Msg:%s", message.c_str());

	} else if (option == PlayerManager::AI_2) {
		// TODO - Implement output
		printf("Msg:%s", message.c_str());
	}
}
