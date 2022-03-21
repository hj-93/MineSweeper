#pragma once
#include <stdexcept>
#include "FieldVisitor.h"

namespace MineSweeper {
    class MineField final
    {
    public:
        class Factory;

        void Accept(IFieldVisitor const& board_vistor) noexcept {
            board_vistor.Visit(board_);
        }

        // Enable move constructor.
        MineField(MineField&&) noexcept = default;

        // Delete other special member functions.
        MineField(MineField const&) = delete;
        MineField operator=(MineField const&) = delete;
        MineField operator=(MineField&&) = delete;
    private:
        // Private constructor to initiate board.
        MineField(int const row_size, int const col_size) noexcept : board_(row_size){
            for (auto& row : board_) {row.resize(col_size);}
        }
        Board board_;
    };

    class MineField::Factory {
    public:
        static MineField CreateCustomizedSize(int const row, int const col) noexcept  {
            return MineField{ row, col };
        }
        static MineField CreateBeginner() {
            throw std::runtime_error(ErrorMsgNotImplemented);
        }
        static MineField CreateIntermediate() {
            throw std::runtime_error(ErrorMsgNotImplemented);
        }
        static MineField CreateExpert() {
            throw std::runtime_error(ErrorMsgNotImplemented);
        }
    };
    
}
