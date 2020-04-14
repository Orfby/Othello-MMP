//Othello headers:
#include <othello/ai/QLearningPlayer.hpp>


namespace othello
{
    
    namespace ai
    {
    
        ////////////////////////////////////////////////////////////////
        FANN::neural_net QLearningPlayer::mlp = {};
        
        
        ////////////////////////////////////////////////////////////////
        QLearningPlayer::QLearningPlayer(const bool& training, const unsigned int& seed,
                const unsigned int& numCycles, const unsigned int& numHiddenLayers,
                const unsigned int& numHiddenNeurons, const fann_type& discountFactor,
                const fann_type& learningRate, const fann_type& epsilon)
                : training(training), randomNumberGenerator(seed),
                  discountFactor(discountFactor), epsilon(epsilon),
                  deltaEpsilon(-epsilon / numCycles)
        {
            //Disable their random number seeding (since we're using our own)
            mlp.disable_seed_rand();
            
            //Create a vector for the layers
            std::vector<unsigned int> layers = {game::Board::BOARD_SIZE * game::Board::BOARD_SIZE};
            //Insert the hidden layers
            for (std::size_t i = 0; i < numHiddenLayers; ++i) {layers.emplace_back(numHiddenNeurons);}
            //Insert the output layer
            layers.emplace_back(game::Board::BOARD_SIZE * game::Board::BOARD_SIZE);
            
            //Create the neural network with the given layers
            mlp.create_standard_array(layers.size(), layers.data());
            //Randomise the weights from -0.5 and 0.5
            mlp.randomize_weights(-0.5f, 0.5f);
            //Set the learning rate
            mlp.set_learning_rate(learningRate);
            //Set the activation function to sigmoid for the hidden and output layer
            mlp.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
            mlp.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
        }
        
        
        ////////////////////////////////////////////////////////////////
        const game::Move* QLearningPlayer::makeMove(const game::Game& game, const uint8_t& player,
                                                        const std::vector<game::Move>& possibleMoves)
        {
            //Create an input array of 0s
            std::array<fann_type, game::Board::BOARD_SIZE * game::Board::BOARD_SIZE> input = {0};
            
            //Iterate over the tiles of the board
            std::size_t i = 0;
            for (uint8_t y = 0; y < game::Board::BOARD_SIZE; ++y)
            {
                for (uint8_t x = 0; x < game::Board::BOARD_SIZE; ++x)
                {
                    //If it's claimed
                    if (game.getBoard().getTile({x, y}).isClaimed)
                    {
                        //Claimed by "me" is 1
                        if (game.getBoard().getTile({x, y}).claimant == player) {input[i] = 1;}
                            //Claimed by the other player is -1
                        else {input[i] = -1;}
                    }
                    else {input[i] = 0;}
                    ++i;
                }
            }
            
            //Run the input into the nn
            auto outputPtr = mlp.run(input.data());
            
            //Create an output array
            std::array<fann_type, game::Board::BOARD_SIZE * game::Board::BOARD_SIZE> output{};
            std::copy(outputPtr, outputPtr + output.size(), output.begin());
            
            //Create some variables for the selected move
            std::size_t selectedMove_possibleMovesI = 0;
            std::size_t selectedMove_nnI = 0;
            fann_type selectedMove_intensity = std::numeric_limits<fann_type>::lowest();
            
            //Iterate over the possible moves to get the one with the strongest signal in the NN
            for (i = 0; i < possibleMoves.size(); ++i)
            {
                //Get the index of the move in the neural network's output
                uint8_t index = (possibleMoves[i].diskPosition.y * game::Board::BOARD_SIZE) +
                                possibleMoves[i].diskPosition.x;
                //If the neural network likes it more
                if (output[index] > selectedMove_intensity)
                {
                    selectedMove_possibleMovesI = i;
                    selectedMove_nnI = index;
                    selectedMove_intensity = output[index];
                }
            }
            
            //If a random move should be picked
            boost::random::uniform_real_distribution<> distribution(0, 1);
            double randomNum = distribution(randomNumberGenerator);
            if (training && randomNum <= epsilon)
            {
                //Create a uniform integer distribution
                boost::random::uniform_int_distribution<> moveDistribution(0, possibleMoves.size() - 1);
                //Get a random move
                selectedMove_possibleMovesI = moveDistribution(randomNumberGenerator);
                
                //Get its index in the nn
                selectedMove_nnI = (possibleMoves[selectedMove_possibleMovesI].diskPosition.y * game::Board::BOARD_SIZE) +
                                   possibleMoves[selectedMove_possibleMovesI].diskPosition.x;
            }
            
            //If we're in training mode
            if (training)
            {
                //Add the state to the backlog
                states.emplace_back(TurnState(input, selectedMove_nnI, selectedMove_intensity));
            }
            
            //Return the selected move
            return &possibleMoves[selectedMove_possibleMovesI];
        }
        
        
        ////////////////////////////////////////////////////////////////
        void QLearningPlayer::gameFinished(const game::Game& game, const uint8_t& player)
        {
            //If we're training
            if (training)
            {
                //Determine the reward
                fann_type reward = 0;
                //If this player won
                if ((player == 0 && game.getScore().first > game.getScore().second) ||
                    (player == 1 && game.getScore().second > game.getScore().first))
                {
                    reward = 1;
                }
                    //If the other player won
                else if ((player == 0 && game.getScore().second > game.getScore().first) ||
                         (player == 1 && game.getScore().first > game.getScore().second))
                {
                    reward = 0;
                }
                    //If it was a draw
                else {reward = 0.5;}
                
                //Iterate over the states
                for (std::size_t i = 0; i < states.size(); ++i)
                {
                    fann_type Qnew;
                    if (i < states.size() - 1)
                    {
                        //Calculate the Q new value
                        Qnew = (discountFactor * states.at(i + 1).QVal);
                    }
                    else {Qnew = reward;}
                    
                    //Run the input into the nn
                    auto QPrevPtr = mlp.run(states.at(i).input.data());
                    
                    //Create an output array
                    std::array<fann_type, game::Board::BOARD_SIZE * game::Board::BOARD_SIZE> QPrev{};
                    std::copy(QPrevPtr, QPrevPtr + QPrev.size(), QPrev.begin());
                    
                    //Adjust the output of the played move
                    QPrev[states.at(i).playedMove] = Qnew;
                    
                    //Train the network
                    mlp.train(states.at(i).input.data(), QPrev.data());
                }
            }
            
            //Clear the state
            states.clear();
        }
        
        
        ////////////////////////////////////////////////////////////////
        void QLearningPlayer::setTraining(const bool& trainingMode)
        {
            //Set the training mode
            training = trainingMode;
            
            //Modify epsilon if training mode is set to false
            if (!training) {epsilon += deltaEpsilon;}
        }
        
    }
    
}