#ifndef OTHELLO_AI_ALPHABETAPRUNINGPLAYER_HPP
#define OTHELLO_AI_ALPHABETAPRUNINGPLAYER_HPP

//Boost headers:
#include <boost/random.hpp>
//Othello headers:
#include <othello/game/IPlayer.hpp>
#include <othello/game/Game.hpp>
#include <othello/util/WorkerThreadManager.hpp>


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
                boost::random::mt19937 randomNumberGenerator;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The alpha-beta recursive algorithm
                ///
                ////////////////////////////////////////////////////////////////
                int64_t alphaBeta(const game::Board& board, const game::Move& move, uint8_t depth,
                        int64_t alpha, int64_t beta);
                
        
                ////////////////////////////////////////////////////////////////
                /// \brief The worker thread manager
                ///
                ////////////////////////////////////////////////////////////////
                util::WorkerThreadManager<int64_t, const game::Board&, const game::Move&,
                        uint8_t, int64_t, int64_t> workerManager;
        
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor that initialises the members
                ///
                /// \param game A const reference to the game
                /// \param player The player number. 0 = player1, 1 = player2
                /// \param searchDepth The search depth of the AI
                /// \param numThreads The number of worker threads
                /// \param seed The seed for the random generator
                ///
                ////////////////////////////////////////////////////////////////
                explicit AlphaBetaPruningPlayer(const game::Game& game, const uint8_t& player,
                        const unsigned int& searchDepth, const uint8_t& numThreads, const unsigned int& seed)
                    : game(game), player(player), searchDepth(searchDepth), randomNumberGenerator(seed),
                        workerManager(std::bind(&AlphaBetaPruningPlayer::alphaBeta, this,
                                std::placeholders::_1, std::placeholders::_2, //I really wish there were a better
                                std::placeholders::_3, std::placeholders::_4, //syntax for doing this
                                std::placeholders::_5), numThreads) {}
                
                
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
                const game::Move* makeMove(const std::vector<game::Move>& possibleMoves) override;
        
        };
        
    }
    
}

#endif //OTHELLO_AI_ALPHABETAPRUNINGPLAYER_HPP
