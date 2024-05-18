#include "surakarta_agent_mine.h"
SurakartaMove SurakartaAgentMine::CalculateMove() {
    // TODO: Implement your own ai here.
    int count = 0;
    std::vector<SurakartaPosition> from;            // 棋子位置数组
    std::vector<SurakartaPosition> to;              // 空位数组
    std::vector<SurakartaPosition> Enemy_Position;  // 敌人的位置
    for (unsigned int i = 0; i < board_size_; i++) {
        for (unsigned int j = 0; j < board_size_; j++) {
            PieceColor color = (*board_)[i][j]->GetColor();
            SurakartaPosition position = {i, j};
            if (color != game_info_->current_player_ && color != PieceColor::NONE) {
                Enemy_Position.push_back(position);
            }
            if (color == game_info_->current_player_) {
                from.push_back(position);
                count++;
            } else {
                to.push_back(position);
            }
        }
    }  // 创建两个vector，from和to，用于存储棋子位置和空位置。遍历游戏棋盘上的每个位置，将符合条件的位置添加到对应的vector中

    int best_score = 0;
    int score;
    int flag = 0;
    int flag1 = 0;
    SurakartaMove best_move({0, 0}, {0, 0}, game_info_->current_player_);

    for (auto& p1 : from) {
        for (auto& p2 : to) {
            SurakartaMove move = {p1, p2, game_info_->current_player_};
            SurakartaIllegalMoveReason reason = rule_manager_->JudgeMove(move);
            score = 0;
            if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                return move;
            } else if (reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                if (count >= 6) {
                    for (auto& p3 : Enemy_Position) {
                        SurakartaMove move2 = {p2, p3, game_info_->current_player_};
                        SurakartaIllegalMoveReason reason = rule_manager_->JudgeMove(move2);
                        if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                            flag1 = 1;
                            break;
                        } else if (reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                            continue;
                        }
                    }
                    if (flag1 == 1) {
                        score -= 20;
                    } else if (flag1 == 0) {
                        score += 5;
                    }
                    flag1 = 0;
                    // 移动到交叉线的棋子分数+7
                    if (move.to.x == board_size_ / 2 && move.to.y == board_size_ / 2) {
                        score += 7;
                    }
                    if (move.to.x == (board_size_ / 2 - 1) && move.to.y == board_size_ / 2) {
                        score += 7;
                    }
                    if (move.to.x == board_size_ / 2 && move.to.y == (board_size_ / 2 - 1)) {
                        score += 7;
                    }
                    if (move.to.x == (board_size_ / 2 - 1) && move.to.y == (board_size_ / 2 - 1)) {
                        score += 7;
                    }
                    if (move.to.x == (board_size_ / 2 - 2) && move.to.y == (board_size_ / 2 - 2)) {
                        score += 7;
                    }
                    if (move.to.x == (board_size_ / 2 - 2) && move.to.y == (board_size_ / 2 + 1)) {
                        score += 7;
                    }
                    if (move.to.x == (board_size_ / 2 + 1) && move.to.y == (board_size_ / 2 - 2)) {
                        score += 7;
                    }
                    if (move.to.x == (board_size_ / 2 + 1) && move.to.y == (board_size_ / 2 + 1)) {
                        score += 7;
                    }
                    // 移动到边界的棋子分数+1
                    if (move.to.x == (board_size_ - 1) && move.to.y == board_size_ - 1) {
                        score += 1;
                    }
                    if (move.to.x == 0 && move.to.y == board_size_ - 1) {
                        score += board_size_ / 2;
                    }
                    if (move.to.x == (board_size_ - 1) && move.to.y == 0) {
                        score += board_size_ / 2;
                    }
                    if (move.to.x == 0 && move.to.y == 0) {
                        score += board_size_ / 2;
                    }
                    // 落在边界线但不是角落的棋子分数+2；
                    if (move.to.x == 0 && move.to.y != 0 && move.to.y != 5) {
                        score += 2;
                    }
                    if (move.to.x == board_size_ - 1 && move.to.y != 0 && move.to.y != 5) {
                        score += 2;
                    }
                    if (move.to.y == 0 && move.to.x != 0 && move.to.x != 5) {
                        score += 2;
                    }
                    if (move.to.y == board_size_ - 1 && move.to.x != 5 && move.to.x != 0) {
                        score += 2;
                    }
                    // 落在其他位置
                    if (move.to.y == board_size_ - 2 && (move.to.x == 1 || move.to.x == 4)) {
                        score += 3;
                    }
                    if (move.to.y == 1 && (move.to.x == 1 || move.to.x == 4)) {
                        score += 3;
                    }
                    if (move.to.x == board_size_ - 2 && (move.to.y == 1 || move.to.y == 4)) {
                        score += 3;
                    }
                    if (move.to.x == 1 && (move.to.y == 1 || move.to.y == 4)) {
                        score += 3;
                    }
                } else {
                    for (auto& p4 : Enemy_Position) {
                        SurakartaMove move2 = {p2, p4, game_info_->current_player_};
                        SurakartaIllegalMoveReason reason = rule_manager_->JudgeMove(move2);
                        if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                            flag = 1;
                            break;
                        } else if (reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                            continue;
                        }
                    }
                    if (flag == 1) {
                        score -= 20;
                    } else if (flag == 0) {
                        score += 5;
                    }
                    flag = 0;
                }
                best_move = (score > best_score ? move : best_move);
                best_score = (score > best_score ? score : best_score);
            }
        }
    }
    return best_move;
}
