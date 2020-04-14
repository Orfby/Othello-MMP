#ifndef OTHELLO_AI_ILEARNINGPLAYER_HPP
#define OTHELLO_AI_ILEARNINGPLAYER_HPP

//Othello headers:
#include <othello/game/IPlayer.hpp>


namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class ILearningPlayer
        ///
        /// \brief Abstract base class representing a single player that
        ///        can learn
        ///
        ////////////////////////////////////////////////////////////////
        class ILearningPlayer : public game::IPlayer
        {
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to either enable or disable training the
                ///        player
                ///
                /// \param trainingMode Whether to enable or disable training
                ///
                ////////////////////////////////////////////////////////////////
                virtual void setTraining(const bool& trainingMode) = 0;
            
        };
        
    }
    
}

#endif //OTHELLO_AI_ILEARNINGPLAYER_HPP
