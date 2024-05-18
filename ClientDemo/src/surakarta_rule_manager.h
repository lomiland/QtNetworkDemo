#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "surakarta_common.h"

class SurakartaRuleManager {
public:
    SurakartaRuleManager() = default;

    SurakartaRuleManager(std::shared_ptr<SurakartaBoard> board,
                         std::shared_ptr<SurakartaGameInfo> game_info)
        : board_size_(board->board_size_),
        board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(std::const_pointer_cast<const SurakartaGameInfo>(game_info)) {}

    unsigned int GetBoardSize() {
        return board_size_;
    }

    virtual void OnUpdateBoard();

    /**
     * @brief Judge whether a move is legal.
     * @param move The move to be judged.
     */
    virtual SurakartaIllegalMoveReason JudgeMove(const SurakartaMove& move);

    /**
     * @brief Judge whether the game is end.
     * @param reason SurakartaIllegalMoveReason of the last move.
     */
    virtual std::pair<SurakartaEndReason, SurakartaPlayer> JudgeEnd(const SurakartaIllegalMoveReason reason);

    /**
     * @brief Get all legal target positions of a piece.
     * @param postion The position of the piece.
     */
    virtual std::unique_ptr<std::vector<SurakartaPosition>> GetAllLegalTarget(const SurakartaPosition postion);

    //    protected:
    unsigned int board_size_;
    std::shared_ptr<const SurakartaBoard> board_;
    std::shared_ptr<const SurakartaGameInfo> game_info_;

public:
    // TODO:
    bool IsAdjacentMove(const SurakartaPosition& from, const SurakartaPosition& to) const {
        bool isAdjacent = false;
        int dx = to.x - from.x;
        int dy = to.y - from.y;
        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1) || (dx == 1 && dy == 1) || (dx == -1 && dy == 0) || (dx == 0 && dy == -1) || (dx == -1 && dy == 1) || (dx == -1 && dy == -1) || (dx == 1 && dy == -1)) {
            isAdjacent = true;
        }
        return isAdjacent;
    }
    bool IsCrossingOuterLineup(SurakartaPosition& from, SurakartaPosition& to) {
        if (from.x == 0 || from.x == board_size_ - 1)
            return false;
        for (int i = from.y - 1; i >= 0; i--) {
            if ((*board_)[from.x][i]->GetColor() != PieceColor::NONE)
                return false;
        }
        if (from.x < board_size_ / 2) {
            int temp_x = 0, temp_y = from.x;
            for (; temp_x < board_size_; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y))
                    return true;
            }
            temp_x = board_size_ - 1 - temp_y;
            temp_y = 0;
            for (; temp_y < board_size_; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y))
                    return true;
            }
            temp_y = temp_x;
            temp_x = board_size_ - 1;
            for (; temp_x >= 0; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y))
                    return true;
            }
            temp_x = board_size_ - 1 - temp_y;
            temp_y = board_size_ - 1;
            for (; temp_y > from.y; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        } else if (from.x >= board_size_ / 2) {
            int temp_x = board_size_ - 1, temp_y = board_size_ - 1 - from.x;
            for (; temp_x >= 0; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = temp_y;
            temp_y = 0;
            for (; temp_y < board_size_; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = board_size_ - 1 - temp_x;
            temp_x = 0;
            for (; temp_x < board_size_; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = temp_y;
            temp_y = board_size_ - 1;
            for (; temp_y > from.y; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        }
    }
    bool IsCrossingOuterLinedown(SurakartaPosition& from, SurakartaPosition& to) {
        if (from.x == 0 || from.x == board_size_ - 1)
            return false;
        for (int i = from.y + 1; i < board_size_; i++) {
            if ((*board_)[from.x][i]->GetColor() != PieceColor::NONE)
                return false;
        }
        if (from.x < board_size_ / 2) {
            int temp_x = 0, temp_y = board_size_ - 1 - from.x;
            for (; temp_x < board_size_; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = temp_y;
            temp_y = board_size_ - 1;
            for (; temp_y >= 0; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = board_size_ - 1 - temp_x;
            temp_x = board_size_ - 1;
            for (; temp_x >= 0; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = temp_y;
            temp_y = 0;
            for (; temp_y < from.y; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        } else if (from.x >= board_size_ / 2) {
            int temp_x = board_size_ - 1, temp_y = from.x;
            for (; temp_x >= 0; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = board_size_ - 1 - temp_y;
            temp_y = board_size_ - 1;
            for (; temp_y >= 0; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = temp_x;
            temp_x = 0;
            for (; temp_x < board_size_; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = board_size_ - 1 - temp_y;
            temp_y = 0;
            for (; temp_y < from.y; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        }
    }
    bool IsCrossingOuterLineleft(SurakartaPosition& from, SurakartaPosition& to) {
        if (from.y == 0 || from.y == board_size_ - 1)
            return false;
        for (int i = from.x - 1; i >= 0; i--) {
            if (((*board_)[i][from.y]->GetColor() != PieceColor::NONE))
                return false;
        }
        if (from.y < board_size_ / 2) {
            int temp_y = 0, temp_x = from.y;
            for (; temp_y < board_size_; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = board_size_ - 1 - temp_x;
            temp_x = 0;
            for (; temp_x < board_size_; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = temp_y;
            temp_y = board_size_ - 1;
            for (; temp_y >= 0; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = board_size_ - 1 - temp_x;
            temp_x = board_size_ - 1;
            for (; temp_x > from.x; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        } else if (from.y >= board_size_ / 2) {
            int temp_y = board_size_ - 1, temp_x = board_size_ - 1 - from.y;
            for (; temp_y >= 0; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = temp_x;
            temp_x = 0;
            for (; temp_x < board_size_; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = board_size_ - 1 - temp_y;
            temp_y = 0;
            for (; temp_y < board_size_; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = temp_x;
            temp_x = board_size_ - 1;
            for (; temp_x > from.x; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        }
    }
    bool IsCrossingOuterLineright(SurakartaPosition& from, SurakartaPosition& to) {
        if (from.y == 0 || from.y == board_size_ - 1)
            return false;
        for (int i = from.x + 1; i < board_size_; i++) {
            if (((*board_)[i][from.y]->GetColor() != PieceColor::NONE))
                return false;
        }
        if (from.y < board_size_ / 2) {
            int temp_y = 0, temp_x = board_size_ - 1 - from.y;
            for (; temp_y < board_size_; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = temp_x;
            temp_x = board_size_ - 1;
            for (; temp_x >= 0; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = board_size_ - 1 - temp_y;
            temp_y = board_size_ - 1;
            for (; temp_y >= 0; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = temp_x;
            temp_x = 0;
            for (; temp_x < from.x; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        } else if (from.y >= board_size_ / 2) {
            int temp_y = board_size_ - 1, temp_x = from.y;
            for (; temp_y >= 0; temp_y--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = board_size_ - 1 - temp_x;
            temp_x = board_size_ - 1;
            for (; temp_x >= 0; temp_x--) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_x = temp_y;
            temp_y = 0;
            for (; temp_y < board_size_; temp_y++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
            temp_y = board_size_ - 1 - temp_x;
            temp_x = 0;
            for (; temp_x < from.x; temp_x++) {
                if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && !(temp_x == to.x && temp_y == to.y) && !(temp_x == from.x && temp_y == from.y))
                    return false;
                else if ((*board_)[temp_x][temp_y]->GetColor() != PieceColor::NONE && (temp_x == to.x && temp_y == to.y)) {
                    return true;
                }
            }
        }
    }
    //  define your own functions/variables here
    void HelloWorld();
    void InlineHelloWorld() {
        std::cout << "Hello World!" << std::endl;
    }

    int bye_world_ = 0;
};
