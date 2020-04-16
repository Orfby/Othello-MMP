//Standard C++:
#include <iostream>
//Boost headers:
#include <boost/program_options.hpp>
//Othello headers:
#include <othello/App.hpp>
#include <othello/game/Game.hpp>
#include <othello/cmd/BoardPrinter.hpp>
#include <othello/cmd/HumanPlayer.hpp>
#include <othello/ai/RandomPlayer.hpp>
#include <othello/ai/HeuristicPlayer.hpp>
#include <othello/ai/AlphaBetaPruningPlayer.hpp>
#include <othello/ai/QLearningPlayer.hpp>


#define OTHELLO_VERSION "1.0"


namespace othello
{
    
    ////////////////////////////////////////////////////////////////
    int App::run(const int& argc, char** argv)
    {
        try
        {
            //The player types
            std::string playerTypes[3];
            //The number of cycles
            unsigned int numCycles;
            //The number of test games
            unsigned int numTestGames;
            //The number of turns to start from during testing
            unsigned int testingStartFromTurn;
            //The search depth
            unsigned int searchDepth;
            //The number of threads
            unsigned int numThreads;
            //The random generator seed
            unsigned int seed;
            //The output file
            std::string outFile;
            
            //Create the general options description object
            boost::program_options::options_description generalOptions("General Options");
            //Add the options
            generalOptions.add_options()
                    ("version,v", "Print the version")
                    ("help,h", "Displays this message")
                    ("help-training,ht", "Displays a message with program options for training")
                    ("help-players,hp", "Displays a message with the players and a description of each")
                    ("help-all", "Displays a message with all the allowed options")
                    ("player1,p1", boost::program_options::value<std::string>(&playerTypes[0])->required(),
                            "The player type that plays first (blacks)")
                    ("player2,p2", boost::program_options::value<std::string>(&playerTypes[1])->required(),
                            "The player type that plays second (whites)")
                    ("hide-board,hb", "Hides the board unless it's a human player's turn")
                    ("possible-moves,pm", "Displays the possible moves")
                    ("num-cycles", boost::program_options::value<unsigned int>(&numCycles)->default_value(100),
                            "The number of cycles of testing (and training if applicable). "
                            "Default is 100")
                    ("num-test-games,nteg", boost::program_options::value<unsigned int>(&numTestGames)->default_value(0),
                            "The number of test games that should be performed each cycle. "
                            "If set to 0, either 1 game will be played or one batch will be played (if --testing-start-from-turn is not 0). "
                            "Default is 0")
                    ("testing-start-from-turn", boost::program_options::value<unsigned int>(&testingStartFromTurn)->default_value(4),
                            "If not 0, during testing each game will start from each possible starting state after the given number of moves. "
                            "If set to 0, all turns start from the default state. "
                            "Recommended for deterministic players such as ai_heur. "
                            "Only applicable if --training is enabled. "
                            "Default is 4")
                    ("search-depth,sd", boost::program_options::value<unsigned int>(&searchDepth)->default_value(3),
                            "The maximum number of moves an AI will look ahead. "
                            "Only applicable to players that try to predict moves, such as ai_ab_minimax. "
                            "Larger numbers exponentially affect speed and memory use. Default is 3")
                    ("num-threads,nt", boost::program_options::value<unsigned int>(&numThreads)->default_value(4),
                            "The number of worker threads an AI will use when determining moves. "
                            "Only applicable to players that use multithreading, such as ai_ab_minimax. "
                            "Default is 4")
                    ("seed,s", boost::program_options::value<unsigned int>(&seed)->default_value(0),
                            "An unsigned integer to be used as the seed for all random number generators. "
                            "A value of 0 will use the current time in seconds since Epoch as the seed. "
                            "Default is 0")
                    ("switch-players", "This option will swap player 1 and 2 every turn. "
                                       "Useful for training")
                    ("out,o", boost::program_options::value<std::string>(&outFile)->default_value("./out.csv"),
                            "The path to the file to output statistics, as a csv. "
                            "Default is ./out.csv");
            
            //Create the player options
            boost::program_options::options_description playerOptions("Player Options");
            //Add the options
            playerOptions.add_options()
                    ("player=human",
                            "A human player that takes moves from the command line. "
                            "Uses the following options: --possible-moves")
                    ("player=ai_random",
                            "A player that uses a random number generator to choose moves. "
                            "Uses the following options: --seed")
                    ("player=ai_heur",
                            "A deterministic player that uses a positional heuristic table to choose moves")
                    ("player=ai_ab_minimax",
                            "A player that uses Alpha Beta Pruning, a positional heuristic table and a random number generator to choose moves. "
                            "Uses the following options: --search-depth, --num-threads, --seed")
                    ("player=ai_q_learn",
                            " A player that uses a multilayer perceptron neural network, Q learning and a random number generator to choose moves. "
                            "Uses the following options: --seed. "
                            "Can be trained by enabling --training. "
                            "Uses the following options while training: --num-hidden-layers, --num-hidden-neurons, --discount-factor, --learning-rate, --epsilon");
    
            //The number of training games
            unsigned int numTrainingGames;
            //The number of turns to start from during training
            unsigned int trainingStartFromTurn;
            
            //The number of hidden layers
            unsigned int numHiddenLayers;
            //The number of hidden neurons
            unsigned int numHiddenNeurons;
            //The discount factor
            double discountFactor;
            //The learning rate
            double learningRate;
            //The probability of the player picking a random move
            double epsilon;
            
            //Lambda to make sure a number is between 0 and 1
            auto between0And1 = [](const double& val)
            {
                if (val < 0 || val > 1)
                {
                    throw std::out_of_range("Value '" +
                    std::to_string(val) + "' should be between 0 and 1");
                }
            };
            
            //Create the training options description object
            boost::program_options::options_description trainingOptions("Training Options");
            //Add the options
            trainingOptions.add_options()
                    ("training,t", "Whether to train the learning players or just test them. "
                                 "Only applicable to players that use learning algorithms, such as ai_q_learn. ")
                    ("test-player,tp", boost::program_options::value<std::string>(&playerTypes[2])->default_value(""),
                            "The player type to play against the player1 during testing. "
                            "Only applicable if --training is enabled. "
                            "If not included, player2 will be used during testing")
                    ("num-training-games,ntrg", boost::program_options::value<unsigned int>(&numTrainingGames)->default_value(20000),
                            "The number of training games that should be performed between tests. "
                            "If set to 0, either 1 game will be played or one batch will be played (if --training-start-from-turn is not 0). "
                            "Only applicable if --training is enabled. "
                            "Default is 20000")
                    ("training-start-from-turn", boost::program_options::value<unsigned int>(&trainingStartFromTurn)->default_value(4),
                            "If not 0, during training each game will start from each possible starting state after the given number of moves. "
                            "If set to 0, all turns start from the default state. "
                            "Recommended for deterministic players such as ai_heur. "
                            "Only applicable if --training is enabled. "
                            "Default is 4")
                    ("num-hidden-layers", boost::program_options::value<unsigned int>(&numHiddenLayers)->default_value(1),
                            "The number of hidden layers in a neural network. "
                            "Only applicable if --training is enabled. "
                            "Only applicable to players with a neural network, such as ai_q_learn. "
                            "Default is 1")
                    ("num-hidden-neurons,nh", boost::program_options::value<unsigned int>(&numHiddenNeurons)->default_value(50),
                            "The number of neurons in each hidden layer of a neural network. "
                            "Only applicable if --training is enabled. "
                            "Only applicable to players with a neural network, such as ai_q_learn. "
                            "Default is 50")
                    ("discount-factor,df",
                            boost::program_options::value<double>(&discountFactor)->default_value(1.0, "1.0")->notifier(between0And1),
                            "A factor between 0 and 1 that discounts later rewards compared to imminent ones."
                            "Only applicable if --training is enabled. "
                            "Only applicable to players that use discount factors, such as ai_q_learn. "
                            "Default is 1.0")
                    ("learning-rate,lr",
                            boost::program_options::value<double>(&learningRate)->default_value(0.01, "0.01")->notifier(between0And1),
                            "A number between 0 and 1 representing the learning rate. "
                            "Only applicable to players that use discount factors, such as ai_q_learn. "
                            "Only applicable if --training is enabled. "
                            "Default is 0.01")
                    ("epsilon,e",
                            boost::program_options::value<double>(&epsilon)->default_value(0.1, "0.1")->notifier(between0And1),
                            "A number between 0 and 1 representing the probability a learning player will pick a random move. "
                            "Will decrease linearly to 0 over training. "
                            "Only applicable to players that use e-greedy exploration, such as ai_q_learn. "
                            "Only applicable if --training is enabled. "
                            "Default is 0.1");
            
            //Create an options description object for all the options
            boost::program_options::options_description allOptions("");
            allOptions.add(generalOptions).add(trainingOptions);
            
            //Create a display options description object for all the options
            boost::program_options::options_description allDisplayOptions("");
            allDisplayOptions.add(generalOptions).add(playerOptions).add(trainingOptions);
            
            //Create the variables map
            boost::program_options::variables_map variablesMap;
            boost::program_options::store(boost::program_options::parse_command_line(
                    argc, argv, allOptions), variablesMap);
    
            //If the user wants help
            if (variablesMap.count("help"))
            {
                std::cout << generalOptions << std::endl;
                return EXIT_SUCCESS;
            }
                //If the user wants help with players
            else if (variablesMap.count("help-players"))
            {
                std::cout << playerOptions << std::endl;
                return EXIT_SUCCESS;
            }
            //If the user wants help with training
            else if (variablesMap.count("help-training"))
            {
                std::cout << trainingOptions << std::endl;
                return EXIT_SUCCESS;
            }
            //If the user wants all the help options
            else if (variablesMap.count("help-all"))
            {
                std::cout << allDisplayOptions << std::endl;
                return EXIT_SUCCESS;
            }
            //If the user wants to know the version
            if (variablesMap.count("version"))
            {
                std::cout << "Program created for final year project "
                             "\"Analysis of Reinforcement Learning Techniques to Play Othello\" "
                             "created by Benjamin S. Hollier (beh43), version " << OTHELLO_VERSION << std::endl;
                return EXIT_SUCCESS;
            }
            
            //Check the options
            boost::program_options::notify(variablesMap);
            
            //Set the seed as the current time if it's 0
            if (seed == 0) {seed = static_cast<unsigned int>(std::time(nullptr));}
            
            //Seed the C random generator
            srand(seed);
            
            //If the game should display possibly moves
            bool displayPossibleMoves = variablesMap.count("possible-moves");
            //If the board should be hidden
            bool hideBoard = variablesMap.count("hide-board");
            //If the players are switched every game
            bool switchPlayers = variablesMap.count("switch-players");
            //If the program is in training mode
            bool training = variablesMap.count("training");
            
            //If the number of training games is 0
            if (numTrainingGames == 0)
            {
                //If it shouldn't start from a later turn,
                //set the number of test games to 1
                if (trainingStartFromTurn == 0) {numTrainingGames = 1;}
            }
            //If the number of test games is 0
            if (numTestGames == 0)
            {
                //If it shouldn't start from a later turn,
                //set the number of test games to 1
                if (testingStartFromTurn == 0) {numTestGames = 1;}
            }
            
            //Create the games
            game::Game testGame;
            game::Game trainingGame;
            
            //If switching is enabled
            if (switchPlayers)
            {
                //Enable switching in both games
                trainingGame.enableSwitchPlayers();
                testGame.enableSwitchPlayers();
            }
            
            //The player objects to be created
            std::shared_ptr<game::IPlayer> players[3] = {nullptr};
            
            //Iterate over the player types
            for (uint8_t i = 0; i < 3; ++i)
            {
                //Get the player type and convert it to lowercase
                std::string playerType = playerTypes[i];
                std::transform(playerType.begin(), playerType.end(), playerType.begin(), ::tolower);
        
                //Only allow non-learners as player 1 if it isn't in training mode
                if ((!training || i != 0) && playerType == "human") {players[i].reset(new othello::cmd::HumanPlayer());}
                else if ((!training || i != 0) && playerType == "ai_random") {players[i].reset(new othello::ai::RandomPlayer(seed));}
                else if ((!training || i != 0) && playerType == "ai_heur") {players[i].reset(new othello::ai::HeuristicPlayer());}
                else if ((!training || i != 0) && playerType == "ai_ab_minimax")
                {
                    players[i].reset(new othello::ai::AlphaBetaPruningPlayer(
                            searchDepth, numThreads, seed));
                }
                else if (playerType == "ai_q_learn") {players[i].reset(new othello::ai::QLearningPlayer(
                        false, seed, numCycles, numHiddenLayers, numHiddenNeurons, discountFactor, learningRate, epsilon));}
                //Only allow the test player to have no player type
                else if (i != 2 || !playerType.empty())
                {
                    std::cerr << "Unknown player type '" << playerTypes[i] << "'" << std::endl;
                    return EXIT_FAILURE;
                }
            }
            
            //If training isn't going to occur
            if (!training)
            {
                //Set the test game players as 1 and 2
                testGame.setPlayers(players[0], players[1]);
            }
            //Otherwise
            else
            {
                //Set the training players as 1 and 2
                trainingGame.setPlayers(players[0], players[1]);
                //If a test player wasn't specified, use player 2
                if (!players[2])
                {
                    players[2] = players[1];
                    playerTypes[2] = playerTypes[1];
                }
                //Set the test game players as 1 and the test player
                testGame.setPlayers(players[0], players[2]);
            }
            
            //Create the info string stream
            std::stringstream infoStrStream;
            infoStrStream << "==============INFO==============" << std::endl;
            #ifdef OTHELLO_DEBUG
            infoStrStream << "Debug? true" << std::endl;
            #else
            infoStrStream << "Debug? false" << std::endl;
            #endif
            infoStrStream << "Version: " << OTHELLO_VERSION << std::endl;
            infoStrStream << "Player 1: " << playerTypes[0] << std::endl;
            infoStrStream << "Player 2: " << playerTypes[1] << std::endl;
            infoStrStream << "Hide board? " << (hideBoard ? "true" : "false") << std::endl;
            infoStrStream << "Display possible moves? " << (displayPossibleMoves ? "true" : "false") << std::endl;
            infoStrStream << "Num cycles: " << numCycles << std::endl;
            infoStrStream << "Num test games: " << numTestGames << std::endl;
            infoStrStream << "Testing start from turn: " << testingStartFromTurn << std::endl;
            infoStrStream << "Search depth: " << searchDepth << std::endl;
            infoStrStream << "Num threads: " << numThreads << std::endl;
            infoStrStream << "Seed: " << seed << std::endl;
            infoStrStream << "Switch players? " << (switchPlayers ? "true" : "false") << std::endl;
            infoStrStream << "Output file: " << outFile << std::endl;
            infoStrStream << std::endl;
            
            if (!training)
            {
                infoStrStream << "Training? false" << std::endl;
            }
            else
            {
                infoStrStream << "Training? true" << std::endl;
                infoStrStream << "Test player: " << playerTypes[2] << std::endl;
                infoStrStream << "Num training games: " << numTrainingGames << std::endl;
                infoStrStream << "Training start from turn: " << trainingStartFromTurn << std::endl;
                infoStrStream << "Num hidden layers: " << numHiddenLayers << std::endl;
                infoStrStream << "Num hidden neurons: " << numHiddenNeurons << std::endl;
                infoStrStream << "Discount factor: " << discountFactor << std::endl;
                infoStrStream << "Learning rate: " << learningRate << std::endl;
                infoStrStream << "Epsilon: " << epsilon << std::endl;
            }
    
            infoStrStream << "================================" << std::endl;
            
            //Print the info string
            std::cout << infoStrStream.str() << std::endl;
            
            //Enable statistics for the test game
            testGame.enableStatistics(outFile, infoStrStream.str());
            
            //Create a pre turn callback
            auto preTurnCallback = [&](const game::Game& game, const uint8_t& player)
            {
                //Create an empty vector of possible moves
                std::vector<game::Move> possibleMoves;
    
                //If the possible moves should be displayed
                if (displayPossibleMoves)
                {
                    //Get the possible moves
                    possibleMoves = game.getBoard().getPossibleMoves();
                }
    
                //If the board shouldn't be hidden or the player is a human
                if (!hideBoard || dynamic_cast<cmd::HumanPlayer*>(players[player - 1].get()) != nullptr)
                {
                    //Print the board
                    cmd::BoardPrinter::print(game.getBoard(), player, possibleMoves);
                }
            };
            
            //Add the pre-turn callback to the games
            trainingGame.addPreTurnCallback(preTurnCallback);
            testGame.addPreTurnCallback(preTurnCallback);
            
            //Create a win callback
            auto winCallback = [&](const game::Game& game, const uint8_t& player)
            {
                //Get the score
                const auto score = game.getScore();
    
                //If the board isn't hidden
                if (!hideBoard)
                {
                    //Print the board
                    cmd::BoardPrinter::print(game.getBoard(), player, {});
        
                    //If it was a draw
                    if (player == 0) {std::cout << "It was a draw!" << std::endl;}
                    else
                    {
                        std::cout << "Player " << (int) player << " (" << (!(bool) (player - 1) ? 'X' : 'O')
                                  << ") won!" << std::endl;
                        std::cout << (unsigned int) score.first << " vs " << (unsigned int) score.second
                                  << std::endl;
                    }
                }
            };
            
            //Add the win callback to the games
            trainingGame.addWinCallback(winCallback);
            testGame.addWinCallback(winCallback);
            
            //Create a lambda for starting games
            //for all the possible moves up to a certain depth
            std::function<void(game::Game&, unsigned int &, unsigned int, const game::Board&)>
                    startGamesForPossibleMoves =
                    [&](game::Game& game, unsigned int& counter, unsigned int depth, const game::Board& board)
                    {
                        //If the depth is 0
                        if (depth == 0)
                        {
                            //Set the board of the game
                            game.setBoard(board);
                            //Play a game
                            game.playGame();
                            //Reset the game
                            game.reset();
                            //Increase the counter
                            ++counter;
                        }
                        else
                        {
                            //Iterate over the possible moves
                            game::Board tmpBoard;
                            for (std::size_t i = 0; i < board.getPossibleMoves().size(); ++i)
                            {
                                //Copy the board
                                tmpBoard = board;
                                //Make the move
                                tmpBoard.makeMove(&tmpBoard.getPossibleMoves()[i]);
                        
                                //Call this function on the possible state
                                startGamesForPossibleMoves(game, counter,
                                                           depth - 1, tmpBoard);
                            }
                        }
                    };
            
            //Iterate over the cycles
            for (unsigned int i = 0; i < numCycles; ++i)
            {
                std::cout << "CYCLE " << i + 1 << std::endl;

                //If we're training
                if (training)
                {
                    //TRAINING
                    std::cout << "Training start" << std::endl;
    
                    //Iterate over the players
                    for (auto& player : players)
                    {
                        //If the player exists and is a learning player
                        if (player && dynamic_cast<othello::ai::ILearningPlayer*>(player.get()))
                        {
                            //Enable training mode
                            dynamic_cast<othello::ai::ILearningPlayer*>(
                                    player.get())->setTraining(true);
                        }
                    }
                    
                    //If it should start from turn 0
                    if (trainingStartFromTurn == 0)
                    {
                        //Iterate over the training games
                        for (unsigned j = 0; j < numTrainingGames; ++j)
                        {
                            //Play a game
                            trainingGame.playGame();
                            //Reset the game
                            trainingGame.reset();
                        }
                    }
                        //If it shouldn't
                    else
                    {
                        //Create a counter for the number of games played
                        unsigned int numGamesPlayed = 0;
                        //Create a temporary counter
                        unsigned int tmpCounter = 0;
                        
                        //Disable (automatic) switching
                        trainingGame.enableSwitchPlayers(false);
                        
                        do
                        {
                            //Create a default board
                            game::Board board;
                            //Start games
                            startGamesForPossibleMoves(trainingGame, tmpCounter,
                                                       trainingStartFromTurn, board);
                            
                            //If the players should be switched
                            if (switchPlayers)
                            {
                                //Switch the players manually
                                trainingGame.manualSwitchPlayers();

                                //Start games
                                startGamesForPossibleMoves(trainingGame, tmpCounter,
                                                           trainingStartFromTurn, board);
                                
                                //Switch the players again
                                trainingGame.manualSwitchPlayers();
                            }
                            //Add to the number of games
                            numGamesPlayed += tmpCounter;
                            //Reset the temporary counter
                            tmpCounter = 0;
                        }
                        while (numGamesPlayed < numTrainingGames);
                        
                        //Reset the switching option
                        trainingGame.enableSwitchPlayers(switchPlayers);
                    }
                }

                {
                    //TESTING
                    std::cout << "Testing start" << std::endl;
                    
                    //Iterate over the players
                    for (auto& player : players)
                    {
                        //If the player exists and is a learning player
                        if (player && dynamic_cast<othello::ai::ILearningPlayer*>(player.get()))
                        {
                            //Disable training mode
                            dynamic_cast<othello::ai::ILearningPlayer*>(
                                    player.get())->setTraining(false);
                        }
                    }
                    
                    //If it should start from turn 0
                    if (testingStartFromTurn == 0)
                    {
                        //Iterate over the test games
                        for (unsigned int j = 0; j < numTestGames; ++j)
                        {
                            //Play a game
                            testGame.playGame();
                            //Reset the game
                            testGame.reset();
                        }
                    }
                        //If it shouldn't
                    else
                    {
                        unsigned int tmpCounter = 0;

                        //Disable (automatic) switching
                        testGame.enableSwitchPlayers(false);
    
                        //Create a counter to count the number of games played
                        unsigned int numGamesPlayed = 0;
                        do
                        {
                            //Create a default board
                            game::Board board;
                            //Start games
                            startGamesForPossibleMoves(testGame, tmpCounter,
                                                       testingStartFromTurn, board);

                            //If the players should be switched
                            if (switchPlayers)
                            {
                                //Switch the players manually
                                testGame.manualSwitchPlayers();
    
                                //Start games
                                startGamesForPossibleMoves(testGame, tmpCounter,
                                                           testingStartFromTurn, board);
    
                                //Switch the players again
                                testGame.manualSwitchPlayers();
                            }
                            
                            //Add to the number of games
                            numGamesPlayed += tmpCounter;
                            //Reset the temporary counter
                            tmpCounter = 0;
                        }
                        while (numGamesPlayed < numTestGames);

                        //Reset the switching option
                        testGame.enableSwitchPlayers(switchPlayers);
                    }
    
                    std::cout << std::endl;
                    testGame.getStatsManager()->output();
                    testGame.getStatsManager()->nextBatch();
                    std::cout << std::endl;
                }
            }
            
            return EXIT_SUCCESS;
        }
        //If a C++ exception was caught
        catch (const std::exception& e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            //Rethrow it just in case
            throw;
        }
        //If *something* was thrown
        catch (...)
        {
            std::cerr << "Caught unknown exception" << std::endl;
            //Rethrow it just in case
            throw;
        }
    }
    
}