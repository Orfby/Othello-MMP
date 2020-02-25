#ifndef OTHELLO_GAME_TILE_HPP
#define OTHELLO_GAME_TILE_HPP

//Standard C++:
#include <cstdint>

namespace othello
{
    
    namespace game
    {
    
        ////////////////////////////////////////////////////////////////
        /// \struct TilePosition
        ///
        /// \brief Simple struct representing the position of a tile on
        ///        an Othello board
        ///
        ////////////////////////////////////////////////////////////////
        struct TilePosition
        {
    
            ////////////////////////////////////////////////////////////////
            /// \brief The x coordinate of the tile on the board (where 0 is
            ///        left-most)
            ///
            ////////////////////////////////////////////////////////////////
            uint8_t x;
    
    
            ////////////////////////////////////////////////////////////////
            /// \brief The y coordinate of the tile on the board (where 0 is
            ///        top-most)
            ///
            ////////////////////////////////////////////////////////////////
            uint8_t y;
            
        };
        
        
        ////////////////////////////////////////////////////////////////
        /// \struct Tile
        ///
        /// \brief Simple struct representing a single tile on an
        ///        Othello board
        ///
        ////////////////////////////////////////////////////////////////
        struct Tile
        {
    
            ////////////////////////////////////////////////////////////////
            /// \brief Whether the tile is free (isClaimed == false) or has
            ///        a disk on it (isClaimed == true)
            ///
            ////////////////////////////////////////////////////////////////
            bool isClaimed;
    
    
            ////////////////////////////////////////////////////////////////
            /// \brief Which player currently has a disk on the tile.
            ///        The value of this is undefined if isClaimed == false.
            ///        claimant == false means player 1 has a disk on the
            ///        tile, claimant == true means player 2
            ///
            /// \note This type could be an uint8_t or an enumerate but
            ///       boolean types at the end of structs are more memory
            ///       efficient due to structure packing (see
            ///       http://www.catb.org/esr/structure-packing/)
            ///
            ////////////////////////////////////////////////////////////////
            bool claimant;
            
        };
        
    }
    
}

#endif //OTHELLO_GAME_TILE_HPP
