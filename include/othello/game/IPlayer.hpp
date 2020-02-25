#ifndef OTHELLO_GAME_IPLAYER_HPP
#define OTHELLO_GAME_IPLAYER_HPP

//Standard C++:
#include <vector>
//Othello headers:
#include <othello/game/Move.hpp>


namespace othello
{
    
    namespace game
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class IPlayer
        ///
        /// \brief Abstract base class representing a single player
        ///
        ////////////////////////////////////////////////////////////////
        class IPlayer
        {
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Virtual default destructor
                ///
                ////////////////////////////////////////////////////////////////
                virtual ~IPlayer() = default;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Virtual function that is called when the player
                ///        should make a move
                ///
                /// \param possibleMoves A vector of the possible moves
                ///
                /// \return A const pointer to a const move that will be played
                ///         by the current player. This pointer must point to a
                ///         move in possibleMoves
                ///
                ////////////////////////////////////////////////////////////////
                virtual const Move* makeMove(const std::vector<Move>& possibleMoves) = 0;
            
        };
    
    }
    
}

#endif //OTHELLO_GAME_IPLAYER_HPP