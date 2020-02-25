#ifndef OTHELLO_AI_MOVEEVALUATOR_HPP
#define OTHELLO_AI_MOVEEVALUATOR_HPP

//Standard C++:
#include <map>
#include <mutex>
//Othello headers:
#include <othello/game/Move.hpp>

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
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Static function to evaluate a move on the given board
                ///
                /// \param board The board after the given move was made
                /// \param move The move to evaluate
                /// \param player Which player's perspective the move should be
                ///        based on
                ///
                /// \return The move's value
                ///
                ////////////////////////////////////////////////////////////////
                static int64_t evaluate(const game::Board& board, const game::Move& move, const uint8_t& player);
            
        };
        
    }
    
}

#endif //OTHELLO_AI_TRANSPOSITIONTABLE_HPP
