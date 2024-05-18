#include <memory>
#include "surakarta_agent_base.h"

class SurakartaAgentMine : public SurakartaAgentBase {
public:
    using SurakartaAgentBase::SurakartaAgentBase;
    SurakartaMove CalculateMove() override;
    SurakartaAgentMine(std::shared_ptr<SurakartaBoard> board,
                       std::shared_ptr<SurakartaGameInfo> game_info,
                       std::shared_ptr<SurakartaRuleManager> rule_manager)
        : SurakartaAgentBase(board, game_info, rule_manager) {
    }
};
