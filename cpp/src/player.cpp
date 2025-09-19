#include "player.h"
#include "player_manager.h"

int Player::makeRequest() { return -1; }

int Player::Prompt() {
	if (option == PlayerManager::GetOption(0)) {
		int id = -1;

		printf("Next move\n> ");
		scanf("%d", &id);
		return id;

	} else if (option == PlayerManager::GetOption(1)) {
		printf("ERROR::PLAYER: Input method for this option: %s is not implemented\n",
			   option.c_str());
		// TODO - Implement input
		return this->makeRequest();

	} else if (option == PlayerManager::GetOption(2)) {
		printf("ERROR::PLAYER: Input method for this option: %s is not implemented\n",
			   option.c_str());
		// TODO - Implement input
		return this->makeRequest();

	} else {
		printf("ERROR::PLAYER: Invalid option name: %s \n", option.c_str());
		exit(-1);
	}
}

void Player::Connect() {}
