#pragma once

#include "player.h"
#include <map>
#include <string>

class PlayerManager {
	static Player *player2;
	static Player *player1;

	static int serverSocket;

	static void check(int res, const char *message);

  public:
	static std::map<std::string, std::string> options;
	static bool RequestHandled;

	static inline std::string Human = "Human";

	static char Curr_player;
	static std::string BoardState;
	static bool ServerRunning;

	static void Init();

	static void StartServer();
	static void CloseServer();
	static void CleanUp();
	static void MsgAll(std::string message);

	static bool CurrPlayerHuman();
	static int MakeMove();
	static std::vector<std::string> GetOptions();
	static std::string GetOption(int id);
	static void PrintOptions();
	static bool ValidOption(char c);

	static void SetPlayers(std::string playerO, std::string playerX);
};
