#ifndef OTHELLO_AI_MOVEEVALUATOR_HPP
#define OTHELLO_AI_MOVEEVALUATOR_HPP

//Standard C++:
#include <map>
#include <mutex>
//Othello headers:
#include <othello/game/Board.hpp>

namespace othello
{
    
    ////////////////////////////////////////////////////////////////
    namespace game {class Board;}
    
    
    namespace ai
    {
        
        ////////////////////////////////////////////////////////////////
        /// \class MoveEvaluator
        ///
        /// \brief Static class for evaluating moves
        ///
        ////////////////////////////////////////////////////////////////
        class MoveEvaluator
        {
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \brief The positional table used to value potential states
                ///
                ////////////////////////////////////////////////////////////////
                static constexpr int table[game::Board::BOARD_SIZE][game::Board::BOARD_SIZE] =
                        {
                                {100, -25, 10, 5, 5, 10, -25, 100},
                                {-25, -25,  2, 2, 2,  2, -25, -25},
                                { 10,   2,  5, 1, 1,  5,   2,  10},
                                {  5,   2,  1, 2, 2,  1,   2,   5},
                                {  5,   2,  1, 2, 2,  1,   2,   5},
                                { 10,   2,  5, 1, 1,  5,   2,  10},
                                { 25, -25,  2, 2, 2,  2, -25, -25},
                                {100, -25, 10, 5, 5, 10, -25, 100}
                        };
                
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Static function to evaluate the given board
                ///
                /// \param board The board after the given move was made
                /// \param player Which player's perspective the move should be
                ///        based on
                ///
                /// \return The board's value
                ///
                ////////////////////////////////////////////////////////////////
                static int64_t evaluate(const game::Board& board, const uint8_t& player);
            
        };
        
    }
    
}

#endif //OTHELLO_AI_TRANSPOSITIONTABLE_HPP
