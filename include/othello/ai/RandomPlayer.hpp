#ifndef OTHELLO_AI_RANDOMPLAYER_HPP
#define OTHELLO_AI_RANDOMPLAYER_HPP

//Boost.random is used instead of the standard library <random>
//as mingw generates the same sequence of random numbers on my machine
//(see https://stackoverflow.com/questions/18880654/why-do-i-get-the-same-sequence-for-every-run-with-stdrandom-device-with-mingw)

//Boost headers:
#include <boost/random.hpp>
//Othello headers:
#include <othello/game/IPlayer.hpp>

namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class RandomPlayer
        ///
        /// \brief An AI player that simply plays a random possible
        ///        move
        ///
        ////////////////////////////////////////////////////////////////
        class RandomPlayer : public game::IPlayer
        {
            private:
                
                ////////////////////////////////////////////////////////////////
                /// \brief A mersenne_twister_engine for generating random
                ///        numbers
                ///
                ////////////////////////////////////////////////////////////////
                boost::random::mt19937 randomNumberGenerator;
                
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor
                ///
                ////////////////////////////////////////////////////////////////
                explicit RandomPlayer(const unsigned int& seed)
                    : randomNumberGenerator(seed) {}
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function that is called when the player should make a
                ///        move. This function simply picks a random move in
                ///        possibleMoves
                ///
                /// \param possibleMoves A vector of the possible moves
                ///
                /// \return A const pointer to a const move that will be played
                ///         by the current player. This pointer must point to a
                ///         move in possibleMoves
                ///
                ////////////////////////////////////////////////////////////////
                const game::Move* makeMove(const game::Game&, const uint8_t&,
                        const std::vector<game::Move>& possibleMoves) override
                {
                    //Create a uniform integer distribution
                    boost::random::uniform_int_distribution<> distribution(0, possibleMoves.size() - 1);
                    //Return a random move
                    return &possibleMoves.at(distribution(randomNumberGenerator));
                }
            
        };
    
    }
    
}

#endif //OTHELLO_AI_RANDOMPLAYER_HPP
