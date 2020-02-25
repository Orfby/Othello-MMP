//Othello headers:
#include <othello/ai/MoveEvaluator.hpp>
#include <othello/game/Board.hpp>

namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        bool MoveEvaluator::BoardCompare::operator()(const game::Board& lhs, const game::Board& rhs)
        {
            return std::lexicographical_compare(std::begin(lhs.grid), std::end(lhs.grid),
                    std::begin(rhs.grid), std::end(rhs.grid));
        }
    
    
        ////////////////////////////////////////////////////////////////
        int64_t MoveEvaluator::evaluate(const othello::game::Board& board, const othello::game::Move& move)
        {
            //Get the current score
            const auto score = board.getScore();
            //If this move ends the game
            if (board.isOver())
            {
                //Return a very high number that is adjusted for the score
                if (board.getCurrentPlayer() == 0) {return ((INT64_MAX - 64) + score.first) - score.second;}
                else {return ((INT64_MAX - 64) + score.second) - score.first;}
            }
            else
            {
                //The initial value is the number of disks
                //the AI has minus the number the other player has
                int64_t moveValue = board.getCurrentPlayer() == 0 ?
                                    score.first - score.second :
                                    score.second - score.first;
        
                //Add the number of possible moves (with a weight of x10)
                moveValue += board.getPossibleMoves().size() * 10;
        
                //The function that is called in each loop
                auto loopFunc = [&](const uint8_t& x, const uint8_t& y)
                {
                    //Get the corner tile
                    game::Tile tile = board.getTile({x, y});
                    //If the tile is claimed by this player, add a point (with a weight of x100)
                    if (tile.isClaimed && tile.claimant == board.getCurrentPlayer()) {moveValue += 100;}
                };
        
                //Iterate over the horizontal sides
                for (uint8_t y = 0; y < game::Board::BOARD_SIZE; y += game::Board::BOARD_SIZE - 1)
                {
                    for (uint8_t x = 0; x < game::Board::BOARD_SIZE; ++x)
                    {
                        loopFunc(x, y);
                    }
                }
        
                //Iterate over the vertical sides
                for (uint8_t y = 0; y < game::Board::BOARD_SIZE; ++y)
                {
                    for (uint8_t x = 0; x < game::Board::BOARD_SIZE; x += game::Board::BOARD_SIZE - 1)
                    {
                        loopFunc(x, y);
                    }
                }
        
                //Return the move's value
                return moveValue;
            }
        }
    
    }
    
}