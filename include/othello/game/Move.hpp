#ifndef OTHELLO_GAME_MOVE_HPP
#define OTHELLO_GAME_MOVE_HPP

//Standard C++:
#include <vector>
//Othello headers:
#include <othello/game/Tile.hpp>

namespace othello
{
    
    namespace game
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class Move
        ///
        /// \brief Class storing a "move" in a game of Othello
        ///
        ////////////////////////////////////////////////////////////////
        class Move
        {
            friend class Board;
            private:
                
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor
                ///
                ////////////////////////////////////////////////////////////////
                Move(uint8_t p, const TilePosition& d,
                        std::vector<TilePosition> f = {})
                        : player(p), diskPosition(d),
                        flippedDiskPositions(std::move(f)) {}
                        
                    
            public:
                
                ////////////////////////////////////////////////////////////////
                /// \brief The index of the player that did the move
                ///
                /// player == 0 is player 1, player == 2 is player 2
                ///
                ////////////////////////////////////////////////////////////////
                uint8_t player;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief A const reference to the tile where the disk is being
                ///        placed
                ///
                ////////////////////////////////////////////////////////////////
                TilePosition diskPosition;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of const references to tiles with disks that
                ///        will be flipped over by this move
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<TilePosition> flippedDiskPositions;
            
        };
        
    }
    
}

#endif //OTHELLO_GAME_MOVE_HPP
