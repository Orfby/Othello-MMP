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
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \struct BoardCompare
                ///
                /// \brief Function structure for comparing two othello boards
                ///
                ////////////////////////////////////////////////////////////////
                struct BoardCompare
                {
    
                    ////////////////////////////////////////////////////////////////
                    /// \brief Function operator
                    ///
                    ////////////////////////////////////////////////////////////////
                    bool operator()(const game::Board& lhs, const game::Board& rhs);
                    
                };
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief The static transposition table
                ///
                ////////////////////////////////////////////////////////////////
                static std::map<game::Board, int64_t, BoardCompare> transpositionTable;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The static mutex to protect the transposition table
                ///
                ////////////////////////////////////////////////////////////////
                std::mutex mutex;
                
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Static function to evaluate a move on the given board
                ///
                /// \param board The board after the given move was made
                /// \param move The move to evaluate
                ///
                /// \return The move's value
                ///
                ////////////////////////////////////////////////////////////////
                static int64_t evaluate(const game::Board& board, const game::Move& move);
            
        };
        
    }
    
}

#endif //OTHELLO_AI_TRANSPOSITIONTABLE_HPP
