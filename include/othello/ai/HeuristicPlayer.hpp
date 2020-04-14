#ifndef OTHELLO_HEURISTICPLAYER_HPP
#define OTHELLO_HEURISTICPLAYER_HPP

//Othello headers:
#include <othello/game/IPlayer.hpp>
#include <othello/game/Game.hpp>
#include <othello/ai/MoveEvaluator.hpp>


namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class HeuristicPlayer
        ///
        /// \brief An AI player that plays moves using a heuristic
        ///        positional table. It is completely deterministic
        ///
        ////////////////////////////////////////////////////////////////
        class HeuristicPlayer : public game::IPlayer
        {
            public:
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function that is called when the player should make a
                ///        move. This function uses a positional table to pick a
                ///        move
                ///
                /// \param game A const reference to the game to make a move in
                /// \param player The index of this player in the game. (0 is
                ///        player 1, 1 is player 2)
                /// \param possibleMoves A vector of the possible moves
                ///
                /// \return A const pointer to a const move that will be played
                ///         by the current player. This pointer must point to a
                ///         move in possibleMoves
                ///
                ////////////////////////////////////////////////////////////////
                const game::Move* makeMove(const game::Game& game, const uint8_t& player,
                        const std::vector<game::Move>& possibleMoves) override
                {
                    std::pair<std::size_t, int64_t> bestMove = {0, INT64_MIN};
                    
                    //todo maybe worker thread manager
                    
                    //Iterate over the possible moves
                    game::Board tmpBoard;
                    int64_t tmpValue;
                    for (std::size_t i = 0; i < game.getBoard().getPossibleMoves().size(); ++i)
                    {
                        //Copy the board
                        tmpBoard = game.getBoard();
                        //Make the move
                        tmpBoard.makeMove(&tmpBoard.getPossibleMoves()[i]);
                        
                        //Calculate its value
                        tmpValue = ai::MoveEvaluator::evaluate(tmpBoard, player);
                        
                        //If it's better
                        if (tmpValue > bestMove.second)
                        {
                            bestMove.first = i;
                            bestMove.second = tmpValue;
                        }
                    }
                    
                    return &possibleMoves[bestMove.first];
                }
            
        };
        
    }
    
}

#endif //OTHELLO_HEURISTICPLAYER_HPP
