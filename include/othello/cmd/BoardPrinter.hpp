#ifndef OTHELLO_CMD_BOARDPRINTER_HPP
#define OTHELLO_CMD_BOARDPRINTER_HPP

//Othello headers:
#include <othello/game/Game.hpp>

namespace othello
{
    
    namespace cmd
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class BoardPrinter
        ///
        /// \brief Class containing a static function for printing an
        ///        Othello game board to the command line
        ///
        ////////////////////////////////////////////////////////////////
        class BoardPrinter
        {
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to print the given board to the command line
                ///
                /// \param board A const reference to the othello board to print
                /// \param player The number of the current player
                /// \param possibleMoves A vector of possible moves. If the
                ///        vector is empty, no possible moves are displayed. By
                ///        default an empty vector
                ///
                ////////////////////////////////////////////////////////////////
                static void print(const game::Board& board, const uint8_t& player,
                        const std::vector<game::Move>& possibleMoves = {});
                
        };
        
    }
    
}

#endif //OTHELLO_CMD_BOARDPRINTER_HPP
