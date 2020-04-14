#ifndef OTHELLO_AI_QLEARNINGPLAYER_HPP
#define OTHELLO_AI_QLEARNINGPLAYER_HPP

//Boost headers:
#include <boost/random.hpp>
//FANN headers:
#include <fann/doublefann.h>
#include <fann/fann_cpp.h>
//Othello headers:
#include <othello/ai/ILearningPlayer.hpp>
#include <othello/game/Game.hpp>


namespace othello
{
    
    namespace ai
    {
        
        ////////////////////////////////////////////////////////////////
        /// \class QLearningPlayer
        ///
        /// \brief A learning AI player that plays moves using Q
        ///        Learning and a multilayer perceptron neural network
        ///
        ////////////////////////////////////////////////////////////////
        class QLearningPlayer : public ILearningPlayer
        {
            private:
                
                ////////////////////////////////////////////////////////////////
                /// \brief Whether the player is in training mode
                ///
                ////////////////////////////////////////////////////////////////
                bool training;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief A mersenne_twister_engine for generating random
                ///        numbers
                ///
                ////////////////////////////////////////////////////////////////
                boost::random::mt19937 randomNumberGenerator;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Static multilayer perceptron network
                ///
                /// The network is static so that multiple agents share the same
                /// network (for self-learning)
                ///
                ////////////////////////////////////////////////////////////////
                static FANN::neural_net mlp;
                
                
                ////////////////////////////////////////////////////////////////
                /// \struct TurnState
                ///
                /// \brief Structure representing the state of the player at a
                ///        certain turn in the game
                ///
                ////////////////////////////////////////////////////////////////
                struct TurnState
                {
                    
                    ////////////////////////////////////////////////////////////////
                    /// \brief The state of the board as an input into the NN
                    ///
                    ////////////////////////////////////////////////////////////////
                    std::array<fann_type, game::Board::BOARD_SIZE * game::Board::BOARD_SIZE> input;
                    
                    
                    ////////////////////////////////////////////////////////////////
                    /// \brief The index in output of the valid move with the
                    ///        highest intensity
                    ///
                    ////////////////////////////////////////////////////////////////
                    std::size_t playedMove;
                    
                    
                    ////////////////////////////////////////////////////////////////
                    /// \brief The Q value of the state
                    ///
                    ////////////////////////////////////////////////////////////////
                    fann_type QVal;
                    
                    
                    ////////////////////////////////////////////////////////////////
                    /// \brief Class constructor
                    ///
                    ////////////////////////////////////////////////////////////////
                    TurnState(std::array<fann_type, game::Board::BOARD_SIZE * game::Board::BOARD_SIZE> input,
                              const std::size_t& playedMove, const fann_type& QVal)
                            : input(input), playedMove(playedMove), QVal(QVal) {}
                    
                };
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief A vector containing the turn states of the on-going
                ///        game
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<TurnState> states;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief The discount factor
                ///
                ////////////////////////////////////////////////////////////////
                const fann_type discountFactor = 1.0;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief The probability for the network to pick a random
                ///        action
                ///
                ////////////////////////////////////////////////////////////////
                fann_type epsilon = 0.1;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief The amount that epsilon should change every training
                ///        cycle
                ///
                ////////////////////////////////////////////////////////////////
                fann_type deltaEpsilon;
            
            
            public:
                
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor
                ///
                /// \param training Whether the player should be in training
                ///        mode
                /// \param seed The seed to be used for random numbers
                /// \param numCycles The number of training cycles. Can be 0.
                /// \param numHiddenLayers The number of hidden layers
                /// \param numHiddenNeurons The number of neurons in each hidden
                ///        layer
                /// \param discountFactor The discount factor (how much it
                ///        discounts later rewards)
                /// \param learningRate How fast the values in the neural
                ///        network change
                /// \param epsilon The probability of the player to select
                ///        random moves during testing. Decreases linerarly over
                ///        the course of training
                ///
                ////////////////////////////////////////////////////////////////
                QLearningPlayer(const bool& training, const unsigned int& seed,
                        const unsigned int& numCycles, const unsigned int& numHiddenLayers,
                        const unsigned int& numHiddenNeurons, const fann_type& discountFactor,
                        const fann_type& learningRate, const fann_type& epsilon);
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function that is called when the player should make a
                ///        move. This function uses a neural network to make a
                ///        decision
                ///
                /// \param game A const reference to the game to make a move in
                /// \param player The index of this player in the game. (0 is
                ///        player 1, 1 is player 2)
                /// \param possibleMoves A vector of the possible moves
                ///
                /// \return A const pointer to a const move that will be played
                ///         by the current player. This pointer must point to a
                ///         move in possibleMoves
                ///
                ////////////////////////////////////////////////////////////////
                const game::Move* makeMove(const game::Game& game, const uint8_t& player,
                                           const std::vector<game::Move>& possibleMoves) override;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Virtual function that is called when a game finishes
                ///
                ////////////////////////////////////////////////////////////////
                void gameFinished(const game::Game&, const uint8_t&) override;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to either enable or disable training the
                ///        player
                ///
                /// \param trainingMode Whether to enable or disable training
                ///
                ////////////////////////////////////////////////////////////////
                void setTraining(const bool& trainingMode) override;
            
        };
        
    }
    
}

#endif //OTHELLO_AI_QLEARNINGPLAYER_HPP
