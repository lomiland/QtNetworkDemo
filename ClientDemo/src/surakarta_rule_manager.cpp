#include "surakarta_rule_manager.h"
#include <iostream>

void SurakartaRuleManager::OnUpdateBoard() {
    // TODO:
    // Every time the board and game_info is updated to the next round version, this function will be called.
    // You don't need to implement this function if you don't need it.
    // A more delicate way is to use Qt's signal and slot mechanism, but that's the advanced part.
}
SurakartaIllegalMoveReason SurakartaRuleManager::JudgeMove(const SurakartaMove& move) {
    // TODO: Implement this function.
    if (move.player != game_info_->current_player_) {
        return SurakartaIllegalMoveReason::NOT_PLAYER_TURN;
    }
    SurakartaPosition from = move.from;
    SurakartaPosition to = move.to;
    if (from.x >= board_size_ || from.y >= board_size_ || to.x >= board_size_ || to.y >= board_size_ || from.x < 0 || from.y < 0 || to.x < 0 || to.y < 0) {
        return SurakartaIllegalMoveReason::OUT_OF_BOARD;
    }
    PieceColor fromColor = (*board_)[from.x][from.y]->GetColor();
    PieceColor toColor = (*board_)[to.x][to.y]->GetColor();
    if (fromColor == PieceColor::NONE) {
        return SurakartaIllegalMoveReason::NOT_PIECE;
    }
    if (fromColor != game_info_->current_player_ && fromColor != PieceColor::NONE) {
        return SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
    }
    if (toColor == PieceColor::NONE) {
        if (SurakartaRuleManager::IsAdjacentMove(from, to)) {
            return SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
        } else {
            return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
        }
    }
    if (toColor == game_info_->current_player_) {
        return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }
    if (toColor != game_info_->current_player_ && toColor != PieceColor::NONE) {
        if (SurakartaRuleManager::IsCrossingOuterLineup(from, to) == true || SurakartaRuleManager::IsCrossingOuterLinedown(from, to) == true || SurakartaRuleManager::IsCrossingOuterLineleft(from, to) == true || SurakartaRuleManager::IsCrossingOuterLineright(from, to) == true) {
            return SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
        } else {
            return SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
        }
    }
    // An example of how to get the color of a piece and the current player.
    for (unsigned int i = 0; i < board_size_; i++) {
        for (unsigned int j = 0; j < board_size_; j++) {
            PieceColor color = (*board_)[i][j]->GetColor();
            if (color == PieceColor::BLACK) {
                // i,j is the position of a black piece
            } else if (color == PieceColor::WHITE) {
                // i,j is the position of a white piece
            } else {
                // i,j is an empty position
            }
        }
    }
    SurakartaPlayer current_player = game_info_->current_player_;
    if (current_player == SurakartaPlayer::BLACK) {
        // black player's turn
    } else if (current_player == SurakartaPlayer::WHITE) {
        // white player's turn
    }

    return SurakartaIllegalMoveReason::LEGAL;
}

std::pair<SurakartaEndReason, SurakartaPlayer> SurakartaRuleManager::JudgeEnd(const SurakartaIllegalMoveReason reason) {
    // TODO: Implement this function.
    int flag = 0;
    if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        for (unsigned int i = 0; i < board_size_; i++) {
            for (unsigned int j = 0; j < board_size_; j++) {
                PieceColor color = (*board_)[i][j]->GetColor();
                if (color != game_info_->current_player_ && color != PieceColor::NONE) {
                    flag++;
                }
            }
        }
    }
    if (flag == 1) {
        return std::make_pair(SurakartaEndReason::CHECKMATE, game_info_->current_player_);
    }
    if (reason != SurakartaIllegalMoveReason::LEGAL && reason != SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE && reason != SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
        return std::make_pair(SurakartaEndReason::ILLIGAL_MOVE, ReverseColor(game_info_->current_player_));
    }
    if (game_info_->num_round_ - game_info_->last_captured_round_ == game_info_->max_no_capture_round_ && reason != SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        int countwhite = 0, countblack = 0;
        for (unsigned int i = 0; i < board_size_; i++) {
            for (unsigned int j = 0; j < board_size_; j++) {
                PieceColor color = (*board_)[i][j]->GetColor();
                if (color == PieceColor::WHITE) {
                    countwhite++;
                } else if (color == PieceColor::BLACK) {
                    countblack++;
                }
            }
        }
        if (countblack > countwhite) {
            return std::make_pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::BLACK);
        } else if (countblack < countwhite) {
            return std::make_pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::WHITE);
        } else {
            return std::make_pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::NONE);
        }
    }
    // Note that at this point, the board and game_info have not been updated yet.

    return std::make_pair(SurakartaEndReason::NONE, SurakartaPlayer::NONE);
}

std::unique_ptr<std::vector<SurakartaPosition>> SurakartaRuleManager::GetAllLegalTarget(const SurakartaPosition postion) {
    // TODO:
    // We don't test this function, you don't need to implement this function if you don't need it.
    return std::make_unique<std::vector<SurakartaPosition>>();
}

void SurakartaRuleManager::HelloWorld() {
    std::cout << "Hello World!" << std::endl;
}
