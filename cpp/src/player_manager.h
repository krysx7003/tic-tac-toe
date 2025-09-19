#pragma once

#include "player.h"
#include <string>
#include <vector>

class PlayerManager {

  public:
	static std::vector<std::string> options;
	static bool RequestHandled;

	static Player player1;
	static Player player2;
	static char Curr_player;

	static void StartServer();
	static void ConnectPlayers();

	static int MakeMove();
	static std::vector<std::string> GetOptions();
	static std::string GetOption(int id);
	static void PrintOptions();
	static bool ValidOption(char c);

	static char GetCurr();

	static void SetCurr(char player);
};
