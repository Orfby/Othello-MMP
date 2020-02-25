#ifndef OTHELLO_CMD_HUMANPLAYER_HPP
#define OTHELLO_CMD_HUMANPLAYER_HPP

//Standard C++:
#include <iostream>
#include <algorithm>
//Othello headers:
#include <othello/game/IPlayer.hpp>


namespace othello
{
    
    namespace cmd
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class HumanPlayer
        ///
        /// \brief A human player that takes input from the command line
        ///
        ////////////////////////////////////////////////////////////////
        class HumanPlayer : public game::IPlayer
        {
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function that is called when the player should make a
                ///        move. This function asks the user for a position in
                ///        the command prompt
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
                    std::string input;
                    while (true)
                    {
                        std::cout << "Please enter the tile coordinate in "
                                     "the form 'cn' (c = character, n = number) to place your disk:" << std::endl;
                        
                        //Get the input and convert it to lowercase
                        std::getline(std::cin, input);
                        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
                        
                        //Take the x character and convert it to a coordinate
                        unsigned int x = input[0] - 'a';
                        //Take the y character and convert it to a coordinate
                        unsigned int y = input[1] - '1';
        
                        //Iterate over the possible moves
                        for (const auto& move : possibleMoves)
                        {
                            //If it's a match
                            if (move.diskPosition.x == x &&
                                move.diskPosition.y == y)
                            {
                                //Return that move
                                return &move;
                            }
                        }
        
                        //Tell the user off
                        std::cout << "Invalid move!" << std::endl;
                        std::cout << "(try using --possible-moves if you "
                                     "don't know where to put pieces)" << std::endl << std::endl;
                    }
                }
                
        };
        
    }
    
}

#endif //OTHELLO_CMD_HUMANPLAYER_HPP
