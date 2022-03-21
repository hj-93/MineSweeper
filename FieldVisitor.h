#pragma once
#include <memory>
#include <numeric>
#include <sstream>
#include <iomanip>
#include "Logger.h"
#include "Common.h"


extern MineSweeper::GameState game_state;

namespace MineSweeper {
    class IFieldVisitor {
    public:
        virtual void Visit(Board&) const noexcept = 0;
        virtual ~IFieldVisitor() noexcept = default;
    };

    class FieldPrinter final : public IFieldVisitor {
    public:
        FieldPrinter(std::unique_ptr<ILogger> logger, bool reveal_all = false) noexcept : logger_{ std::move(logger) }, reveal_all_{ reveal_all }{}

        void Visit(Board& board) const noexcept final {
            int const max_row_index_digit_count = std::to_string(board.size() - 1).length();
            int const max_col_index_digit_count = std::to_string(board[0].size() - 1).length();

            // Print head row
            std::ostringstream oss{};
            oss << std::setw(max_row_index_digit_count + 1) << " ";
            for (int i = 0; i < board[0].size(); ++i) {
                oss << std::setw(max_col_index_digit_count + 1) << std::to_string(i);
            }
            logger_->LogInfo(oss.str()); 

            // Print rows of board
            for (auto row = board.crbegin(); row != board.crend(); ++row) {
                std::ostringstream oss{};
                oss << std::setw(max_row_index_digit_count + 1) << (std::to_string(board.crend() - 1 - row) + "|");
                for (auto& tile: *row) {
                    oss << std::setw(max_col_index_digit_count + 1) << ((tile.visited || reveal_all_) ? tile.value : UNVISITED);
                }
                logger_->LogInfo(oss.str());
            }
        }
        ~FieldPrinter() noexcept final = default;
    private:
        std::unique_ptr<ILogger> logger_;
        bool reveal_all_;
    };

    class BombSetter final : public IFieldVisitor {
    public:
        BombSetter(int x, int y) noexcept : x_{ x }, y_{ y } {}

        void Visit(Board& board) const noexcept final {
               
            if (true == ValidCoord(board, x_, y_) && MINE != board[y_][x_].value) {
                auto& current_tile = board[y_][x_];
                // Neigbor index boundary.
                int const up = y_ == 0 ? 0 : (y_ - 1);
                int const down = y_ == (board.size() - 1) ? y_ : (y_ + 1);
                int const left = x_ == 0 ? 0 : (x_ - 1);
                int const right = x_ == (board[0].size() - 1) ? x_ : (x_ + 1);

                // Update neigbor digit when applicable.
                for (auto row = up; row <= down; ++row) {
                    for (auto col = left; col <= right; ++col) {
                        auto& neighbor = board[row][col];
                        if (MINE != neighbor.value) {
                            ++neighbor.value;
                        }
                    }
                }

                // Set current tile to MINE.
                current_tile.value = MINE;
            }
        }
        ~BombSetter() noexcept final = default;
    private:
        int x_;
        int y_;
    };

    class TileOpener final : public IFieldVisitor {
    public:
        TileOpener(int x, int y) noexcept : x_{ x }, y_{ y } {}

        void DFSUncoverNeighbor(Board& board, int row_id, int col_id) const noexcept {
            // Neigbor index boundary.
            int const up = row_id == 0 ? 0 : (row_id - 1);
            int const down = row_id == (board.size() - 1) ? row_id : (row_id + 1);
            int const left = col_id == 0 ? 0 : (col_id - 1);
            int const right = col_id == (board[0].size() - 1) ? col_id : (col_id + 1);

            // DFS to uncover as many as possible.
            for (auto row = up; row <= down; ++row) {
                for (auto col = left; col <= right; ++col) {
                    auto& current_tile = board[row][col];
                    if (false == current_tile.visited && MINE != current_tile.value) {
                        current_tile.visited = true;
                        if (ZERO == current_tile.value) {
                            DFSUncoverNeighbor(board, row, col);
                        }
                    }
                }
            }
        }

        void Visit(Board& board) const noexcept final {
            if (true == ValidCoord(board, x_, y_) && false == board[y_][x_].visited) {
                auto& current_tile = board[y_][x_];
                current_tile.visited = true;
                if (ZERO == current_tile.value) {
                    DFSUncoverNeighbor(board, y_, x_);
                }
                else if (MINE == current_tile.value) {
                    game_state = GameState::GameOver;
                } 
            }
        }
        ~TileOpener() noexcept final = default;
    private:
        int x_;
        int y_;
    };

    class GameWonJudger final : public IFieldVisitor {
    public:

        void Visit(Board& board) const noexcept final {
            bool game_won{ true };
            for (auto& row : board) {
                auto find_any_unsivisted = std::find_if(row.begin(), row.end(), [](Tile& tile) { return  MINE != tile.value && false == tile.visited; });
                if (find_any_unsivisted != row.end()) { // unvisited non-mine tile found
                    game_won = false;
                    break;
                }
            }
            if (game_won) {
                game_state = GameState::GameWon;
            }
        }
        ~GameWonJudger() noexcept final = default;
    };
}