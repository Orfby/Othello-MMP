//Standard C++:
#include <iostream>
//Othello headers:
#include <othello/cmd/BoardPrinter.hpp>

namespace othello
{
    
    namespace cmd
    {
    
        ////////////////////////////////////////////////////////////////
        void BoardPrinter::print(const game::Board& board, const uint8_t& player,
                const std::vector<game::Move>& possibleMoves)
        {
            //Print the board's state
            std::cout << "Player " << (int)player << ":" << std::endl;
            std::cout << "    a | b | c | d | e | f | g | h " << std::endl;
            for (uint8_t y = 0; y < othello::game::Board::BOARD_SIZE; ++y)
            {
                std::cout << y + 1 << " | ";
                for (uint8_t x = 0; x < othello::game::Board::BOARD_SIZE; ++x)
                {
                    //If this tile doesn't have a disk
                    if (!board.getTile({x, y}).isClaimed)
                    {
                        //Find out if this tile is a possible move
                        bool foundPossible = false;
                        for (const othello::game::Move& move : possibleMoves)
                        {
                            //If the move is a match
                            if (move.diskPosition.x == x && move.diskPosition.y == y)
                            {
                                //Display the cell as a question mark
                                std::cout << "? | ";
                                foundPossible = true;
                                break;
                            }
                        }
                
                        //If it isn't a possible move, the cell should be empty
                        if (!foundPossible) {std::cout << "  | ";}
                    }
                    else
                    {
                        //Player 1 is 0
                        if (!board.getTile({x, y}).claimant) {std::cout << "X | ";}
                            //Player 2 is X
                        else {std::cout << "O | ";}
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        
    }
    
}