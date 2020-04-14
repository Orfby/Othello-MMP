#ifndef OTHELLO_GAME_BOARD_HPP
#define OTHELLO_GAME_BOARD_HPP

//Standard C++:
#include <cstdint>
#include <array>
//Othello headers
#include <othello/game/Move.hpp>


namespace othello
{
    
    namespace game
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class Board
        ///
        /// \brief Class that holds and manages an Othello board
        ///
        ////////////////////////////////////////////////////////////////
        class Board
        {
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief The size of a board
                ///
                ////////////////////////////////////////////////////////////////
                static constexpr std::uint8_t BOARD_SIZE = 8;
        
        
                ////////////////////////////////////////////////////////////////
                static_assert(BOARD_SIZE % 2 == 0, "An Othello board must be an even size");
                
                
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \brief The game board
                ///
                ////////////////////////////////////////////////////////////////
                Tile grid[BOARD_SIZE][BOARD_SIZE] = {};
        
                
                ////////////////////////////////////////////////////////////////
                /// \brief The index of the current player (either 0 or 1)
                ///
                ////////////////////////////////////////////////////////////////
                uint8_t currentPlayer = 0;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of the possible moves for each player
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<Move> possibleMoves[2];
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The current score
                ///
                ////////////////////////////////////////////////////////////////
                std::pair<uint8_t, uint8_t> score = {2, 2};
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Whether the game on the board is done
                ///
                ////////////////////////////////////////////////////////////////
                bool finished = false;
        
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to update possiblesMoves for the current
                ///        state of the board
                ///
                ////////////////////////////////////////////////////////////////
                void calculatePossibleMoves(Move previousMove);
            
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor that initialises the board
                ///
                ////////////////////////////////////////////////////////////////
                Board();
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Const function to get a const reference to a tile at
                ///        the given position
                ///
                /// \warning Doesn't check if pos.x/pos.y is out of bounds
                ///
                ////////////////////////////////////////////////////////////////
                const Tile& getTile(const TilePosition& pos) const;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the player who's turn it currently is
                ///
                /// \return A const reference to the index of the player who's
                ///         turn it is (0 = player 1, 1 = player2)
                ///
                ////////////////////////////////////////////////////////////////
                const uint8_t& getCurrentPlayer() const;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the possible moves for the current
                ///        player
                ///
                /// \return A const reference to the possible moves for this
                ///         player. The vector this points to will change every
                ///         time it's the current player's turn
                ///
                ////////////////////////////////////////////////////////////////
                const std::vector<Move>& getPossibleMoves() const;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to play the next turn
                ///
                /// \note This function will skip a turn if there are no
                ///       possible moves
                /// \note The board is not automatically reset after a game ends
                ///
                /// \param move A const pointer to a const move that will be
                ///        played by the current player. This pointer must point
                ///        to a move in getPossibleMoves
                ///
                /// \return Whether this move ended the game
                ///
                ////////////////////////////////////////////////////////////////
                bool makeMove(const Move* move);
        
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the board's current score (number of
                ///        disks for each player)
                ///
                /// \return A const reference pair where the first value is
                ///         player 1's score and the second is player 2's score
                ///
                ////////////////////////////////////////////////////////////////
                const std::pair<uint8_t, uint8_t>& getScore() const;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to determine if the game on the board is
                ///        over
                ///
                ////////////////////////////////////////////////////////////////
                bool isOver() const;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to reset the board
                ///
                ////////////////////////////////////////////////////////////////
                void reset();
            
        };
        
    }
    
}

#endif //OTHELLO_GAME_BOARD_HPP
