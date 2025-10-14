#include "player_manager.h"
#include "core/resource_manager.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKET_ERROR (-1)
#define SERVER_SOCKET 8080

std::map<std::string, std::string> PlayerManager::options = {{PlayerManager::Human, "echo ERROR"}};

bool PlayerManager::RequestHandled = false;

Player *PlayerManager::player1 = new Player(Human, Player::O, options[Human]);
Player *PlayerManager::player2 = new Player(Human, Player::X, options[Human]);
char PlayerManager::Curr_player = Player::O;
std::string PlayerManager::BoardState = "/ / / / / / / / / /";

int PlayerManager::serverSocket;
bool PlayerManager::ServerRunning = false;

void PlayerManager::Init() {
	json config = ResourceManager::LoadConfig();
	json players = config["players"];

	for (auto &player : players) {
		std::string name = player["name"];
		std::string cmd = player["cmd"];

		options.insert({name, cmd});
	}
}

std::vector<std::string> PlayerManager::GetOptions() {
	std::vector<std::string> keys;
	keys.push_back(Human);
	for (const auto &pair : options) {
		if (pair.first != Human)
			keys.push_back(pair.first);
	}
	return keys;
}

void PlayerManager::PrintOptions() {
	std::vector<std::string> keys = GetOptions();
	for (int i = 0; i < keys.size(); i++) {
		printf("%d. %s\n", i, keys[i].c_str());
	}
}

std::string PlayerManager::GetOption(int id) {
	std::vector<std::string> keys = GetOptions();
	return keys[id];
}

bool PlayerManager::ValidOption(char c) {
	if (c > '0' && c < '9')
		return false;

	int id = c - '0';
	if (id >= options.size())
		return false;

	return true;
}

void PlayerManager::StartServer() {
	if (!ServerRunning) {
		check((serverSocket = socket(AF_INET, SOCK_STREAM, 0)), "Socket creation failed!\n");

		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(SERVER_SOCKET);
		serverAddress.sin_addr.s_addr = INADDR_ANY;

		check(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)),
			  "Bind failed");

		check(listen(serverSocket, 10), "Listen failed");
		ServerRunning = true;
	}

	if (player1->Run()) {
		check((player1->Socket = accept(serverSocket, nullptr, nullptr)), "Accept failed");
		player1->Msg("NAME:O\n");
	} else {
		player1->Msg("RESTART\n");
	}

	if (player2->Run()) {
		check((player2->Socket = accept(serverSocket, nullptr, nullptr)), "Accept failed");
		player2->Msg("NAME:X\n");
	} else {
		player2->Msg("RESTART\n");
	}
}

void PlayerManager::CloseServer() {
	if (ServerRunning)
		close(serverSocket);
}

void PlayerManager::CleanUp() {
	CloseServer();
	player1->CleanUp();
	player2->CleanUp();
}

void PlayerManager::check(int res, const char *message) {
	if (res == SOCKET_ERROR) {
		std::string errorMessage = std::string(message) + ". Exiting...";
		printf("%s\n", errorMessage.c_str());
		close(serverSocket);
		exit(-1);
	}
}

int PlayerManager::MakeMove() {
	if (Curr_player == Player::O) {
		return player1->Prompt();
	} else {
		return player2->Prompt();
	}
}

bool PlayerManager::CurrPlayerHuman() {
	if (Curr_player == Player::O) {
		return player1->Name == Human;
	} else {
		return player2->Name == Human;
	}
}

void PlayerManager::SetPlayers(std::string playerO, std::string playerX) {
	if (player1->Name != playerO) {
		delete player1;
		player1 = nullptr;
		player1 = new Player(playerO, Player::O, options[playerO]);
	}

	if (player2->Name != playerX) {
		delete player2;
		player2 = nullptr;
		player2 = new Player(playerX, Player::X, options[playerX]);
	}
}

void PlayerManager::MsgAll(std::string message) {
	player1->Msg(message);
	player2->Msg(message);
}
