#pragma once
#include <vector>

namespace MineSweeper {
	static const char* const ErrorMsgNotImplemented = "Not Implemented!";
	static constexpr char UNVISITED = '?';
	static constexpr char MINE = 'X';
	static constexpr char ZERO = '0';

	enum class GameState: std::uint8_t {
		GamePlaying,
		GameWon,
		GameOver
	};

	struct Tile {
		char value{ ZERO };
		bool visited{ false };
	};

	using Board = std::vector<std::vector<Tile>>;

	static bool ValidCoord(Board& board, int x, int y) {
		bool res = x < board[0].size() && y < board.size();
		if (false == res) {
			ConsoleLogger{}.LogInfo("Invalid Coordinates!");
		}
		return res;
	}
}