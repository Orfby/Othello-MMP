//Othello headers:
#include <othello/ai/AlphaBetaPruningPlayer.hpp>
#include <othello/ai/MoveEvaluator.hpp>

namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        int64_t AlphaBetaPruningPlayer::alphaBeta(const game::Board& board, const game::Move& move, uint8_t depth,
                          int64_t alpha, int64_t beta)
        {
            //If we've reached the max is depth or this is the last move
            if (depth == 0 || board.isOver())
            {
                return ai::MoveEvaluator::evaluate(board, move, player);
            }
        
            //Whether we're maximising (whether we want the best or worst move)
            bool maximising = board.getCurrentPlayer() == player;
        
            //Create a temporary board
            game::Board tmpBoard;
            int64_t value = maximising ? INT64_MIN : INT64_MAX;
        
            //Iterate over the possible moves
            for (std::size_t i = 0; i < board.getPossibleMoves().size(); ++i)
            {
                //Copy the board
                tmpBoard = board;
                //Make the move
                tmpBoard.makeMove(&tmpBoard.getPossibleMoves()[i]);
                //If we want to maximise the value
                if (maximising)
                {
                    value = std::max(value, alphaBeta(tmpBoard, board.getPossibleMoves()[i],
                                                      depth - 1, alpha, beta));
                    alpha = std::max(alpha, value);
                }
                    //If it's the other player, we want the min
                else
                {
                    value = std::min(value, alphaBeta(tmpBoard, board.getPossibleMoves()[i],
                                                      depth - 1, alpha, beta));
                    beta = std::min(beta, value);
                }
                //If the alpha is larger than the beta, break out of the loop
                if (alpha >= beta) {break;}
            }
            return value;
        }
    
        
        ////////////////////////////////////////////////////////////////
        const game::Move* AlphaBetaPruningPlayer::makeMove(const std::vector<game::Move>& possibleMoves)
        {
            //A map of move indexes with the associated move value as the key
            //(the index being the key means the data is automatically sorted)
            std::map<int64_t, std::size_t> movesAndValues;
            
            //Iterate over the possible moves
            game::Board tmpBoard;
            for (std::size_t i = 0; i < game.getBoard().getPossibleMoves().size(); ++i)
            {
                //Copy the board
                tmpBoard = game.getBoard();
                //Make the move
                tmpBoard.makeMove(&tmpBoard.getPossibleMoves()[i]);
                //Calculate the move's value and put it in the map
                movesAndValues.emplace(
                        alphaBeta(tmpBoard, game.getBoard().getPossibleMoves()[i],
                                searchDepth,INT64_MIN, INT64_MAX), i);
            }
    
            //Create a vector of the best moves
            std::vector<std::size_t> bestMoves;
    
            //Get the best move value
            int64_t bestMoveValue = movesAndValues.rbegin()->first;
            
            //Iterate over the sorted moves backwards
            for (auto it = movesAndValues.rbegin(); it != movesAndValues.rend(); ++it)
            {
                //If the value is within 20 "points" of the best value
                if (bestMoveValue - it->first <= 20)
                {
                    //Add the move
                    bestMoves.emplace_back(it->second);
                }
                //Otherwise break out of the loop
                else {break;}
            }
            
            //Pick a random move from the best moves
            boost::random::uniform_int_distribution<> distribution(0, bestMoves.size() - 1);
            return &possibleMoves[bestMoves[distribution(randomNumberGenerator)]];
        }
        
    }
    
}