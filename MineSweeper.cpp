#include <iostream>
#include <string>

#include "MineField.h"

// Game state global variable.
MineSweeper::GameState game_state{ MineSweeper::GameState::GamePlaying };

int main(int argc, char** argv)
{
	using namespace MineSweeper;

	int const row_size{20};
	int const col_size{20};
	MineField minefield{ MineField::Factory::CreateCustomizedSize(row_size, col_size)};

	// set the bombs...
	minefield.Accept(BombSetter{ 0, 0 });
	minefield.Accept(BombSetter{ 0, 1 });
	minefield.Accept(BombSetter{ 1, 1 });
	minefield.Accept(BombSetter{ 1, 4 });
	minefield.Accept(BombSetter{ 4, 2 });

	// set more bombs...
	minefield.Accept(BombSetter{ 18, 8 });
	minefield.Accept(BombSetter{ 14, 12 });
	minefield.Accept(BombSetter{ 16, 10 });
	minefield.Accept(BombSetter{ 9, 16 });
	minefield.Accept(BombSetter{ 9, 17 });
	minefield.Accept(BombSetter{ 10, 16 });
	minefield.Accept(BombSetter{ 13, 2 });
	minefield.Accept(BombSetter{ 14, 2 });
	minefield.Accept(BombSetter{ 15, 2 });

	std::unique_ptr<ILogger> logger{ std::make_unique<ConsoleLogger>() };
	
	// Debug line: see the entire board.
	// minefield.Accept(FieldPrinter{ std::make_unique<ConsoleLogger>(), true });

	logger->LogInfo("Game start ...");
	FieldPrinter field_printer{ std::make_unique<ConsoleLogger>() };
	minefield.Accept(field_printer);

	GameWonJudger game_won_judger{};

	while (game_state == GameState::GamePlaying) {
		logger->LogInfo("Uncover a tile by coordinate, e.g. '4 0'");
		std::string in;
		std::getline(std::cin, in);
		std::size_t space_pos = in.find(' ');
		int x = std::stoi(in.substr(0, space_pos));
		int y = std::stoi(in.substr(space_pos + 1, in.size() - space_pos));

		minefield.Accept(TileOpener{ x, y });
		minefield.Accept(game_won_judger);
		minefield.Accept(field_printer);

		if (game_state == GameState::GameOver) {
			logger->LogInfo("Game over! Bomb is hitted!");
		}
		else if (game_state == GameState::GameWon) {
			logger->LogInfo("Congratulations!!!");
		}
	}
	return 0;
}
