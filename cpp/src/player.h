#pragma once

#include <string>
#include <vector>

class Player {
	static int promptPlayer(std::string player);
	static int makeRequest();

  public:
	static std::vector<std::string> options;
	static bool RequestHandled;

	static const char O = 'O';
	static const char X = 'X';

	static std::string player1;
	static std::string player2;

	static int MakeMove(char player);
	static std::vector<std::string> GetOptions();
	static std::string GetOption(int id);
	static void PrintOptions();
	static bool ValidOption(char c);
};
