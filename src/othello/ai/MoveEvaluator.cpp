//Othello headers:
#include <othello/ai/MoveEvaluator.hpp>
#include <othello/game/Board.hpp>

namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        constexpr int MoveEvaluator::table[game::Board::BOARD_SIZE][game::Board::BOARD_SIZE];
        
        
        ////////////////////////////////////////////////////////////////
        int64_t MoveEvaluator::evaluate(const othello::game::Board& board, const uint8_t& player)
        {
            //The value of the board
            int64_t value = 0;
            
            //Calculate the value
            for (uint8_t y = 0; y < game::Board::BOARD_SIZE; ++y)
            {
                for (uint8_t x = 0; x < game::Board::BOARD_SIZE; ++x)
                {
                    if (board.getTile({x, y}).isClaimed)
                    {
                        int8_t c = board.getTile({x, y}).claimant == player ? 1 : -1;
                        value += table[y][x] * c;
                    }
                }
            }
            
            return value;
        }
    
    }
    
}