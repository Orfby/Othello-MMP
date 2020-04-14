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
        class Game;
        
        
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
                virtual const Move* makeMove(const game::Game& game, const uint8_t& player,
                        const std::vector<Move>& possibleMoves) = 0;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Virtual function that is called when a game finishes
                ///
                /// \param game The game that finished
                /// \param player The index of this player. NOT the player that
                ///        won. (0 is player 1, 1 is player 2)
                ///
                ////////////////////////////////////////////////////////////////
                virtual void gameFinished(const class Game& game, const uint8_t& player) {}
            
        };
    
    }
    
}

#endif //OTHELLO_GAME_IPLAYER_HPP