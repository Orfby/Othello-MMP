#ifndef OTHELLO_AI_ALPHABETAPRUNINGPLAYER_HPP
#define OTHELLO_AI_ALPHABETAPRUNINGPLAYER_HPP

//Boost headers:
#include <boost/random.hpp>
//Othello headers:
#include <othello/game/IPlayer.hpp>
#include <othello/game/Game.hpp>
#include <othello/ai/MoveEvaluator.hpp>


namespace othello
{
    
    namespace ai
    {
        
        ////////////////////////////////////////////////////////////////
        /// \class AlphaBetaPruningPlayer
        ///
        /// \brief An AI player that plays moves using the negamax and
        ///        alpha-beta pruning algorithms
        ///
        ////////////////////////////////////////////////////////////////
        class AlphaBetaPruningPlayer : public game::IPlayer
        {
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \brief A const reference to the game the AI is playing in
                ///
                ////////////////////////////////////////////////////////////////
                const game::Game& game;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Which player the AI is
                ///
                ////////////////////////////////////////////////////////////////
                uint8_t player;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The search depth in number of moves ahead
                ///
                ////////////////////////////////////////////////////////////////
                unsigned int searchDepth;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief A mersenne_twister_engine for generating random
                ///        numbers
                ///
                ////////////////////////////////////////////////////////////////
                boost::random::mt19937 randomNumberGenerator{static_cast<unsigned int>(std::time(nullptr))};
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The alpha-beta recursive algorithm
                ///
                ////////////////////////////////////////////////////////////////
                int64_t alphaBeta(const game::Board& board, const game::Move& move, uint8_t depth,
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
        
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor that initialises the members
                ///
                /// \param game A const reference to the game
                /// \param player The player number. 0 = player1, 1 = player2
                /// \param searchDepth The search depth of the AI
                ///
                ////////////////////////////////////////////////////////////////
                explicit AlphaBetaPruningPlayer(const game::Game& game, const uint8_t& player, const unsigned int& searchDepth)
                    : game(game), player(player), searchDepth(searchDepth) {}
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function that is called when the player should make a
                ///        move. This function uses the negamax and alpha-beta
                ///        pruning algorithms to make a decision
                ///
                /// \param possibleMoves A vector of the possible moves
                ///
                /// \return A const pointer to a const move that will be played
                ///         by the current player. This pointer must point to a
                ///         move in possibleMoves
                ///
                ////////////////////////////////////////////////////////////////
                const game::Move* makeMove(const std::vector<game::Move>& possibleMoves) override
                {
                    //Create a temp board
                    game::Board tmpBoard;
                    
                    std::pair<std::size_t, int64_t> bestMove = {0, INT64_MIN};
                    int64_t value;
                    
                    //Iterate over the possible moves
                    for (std::size_t i = 0; i < game.getBoard().getPossibleMoves().size(); ++i)
                    {
                        //Copy the board
                        tmpBoard = game.getBoard();
                        //Make the move
                        tmpBoard.makeMove(&tmpBoard.getPossibleMoves()[i]);
                        //Calculate the move's value
                        value = alphaBeta(tmpBoard, game.getBoard().getPossibleMoves()[i], searchDepth,
                                          INT64_MIN, INT64_MAX);
                        //If it's the best move
                        if (value > bestMove.second)
                        {
                            //Set it as such
                            bestMove.first = i;
                            bestMove.second = value;
                            
                            //add some randomness here
                        }
                        //If it's equally as good
                        else if (value == bestMove.second)
                        {
                            //Flip a coin (essentially)
                            boost::random::uniform_int_distribution<> distribution(0, 1);
                            if ((bool)distribution(randomNumberGenerator))
                            {
                                //Set this as the new best move
                                bestMove.first = i;
                                bestMove.second = value;
                                //Doing this makes the AI less
                                //predictable for an RL/DL AI
                            }
                        }
                    }
                    
                    //Return the best move
                    return &possibleMoves.at(bestMove.first);
                }
        
        };
        
    }
    
}

#endif //OTHELLO_AI_ALPHABETAPRUNINGPLAYER_HPP
