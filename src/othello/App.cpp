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
#include <othello/ai/AlphaBetaPruningPlayer.hpp>


namespace othello
{
    
    ////////////////////////////////////////////////////////////////
    int App::run(const int& argc, char** argv)
    {
        try
        {
            //The player types
            std::string playerTypes[2];
            //The number of games to play for
            unsigned int numGames;
            //The search depth
            unsigned int searchDepth;
            //The number of threads
            unsigned int numThreads;
            
            //Create the options description object
            boost::program_options::options_description description("Options");
            //Add the options
            description.add_options()
                    ("version,v", "Print the version")
                    ("help,h", "Displays this message")
                    ("player1,p1", boost::program_options::value<std::string>(&playerTypes[0])->required(),
                            "The player type that plays first (blacks)")
                    ("player2,p2", boost::program_options::value<std::string>(&playerTypes[1])->required(),
                            "The player type that plays second (whites)")
                    ("gui,g", "Starts the GUI interface for the program (unimplemented)")
                    ("hide-board,hb", "Hides the board unless it's a human player's turn")
                    ("possible-moves,pm", "Displays the possible moves")
                    ("num-games,ng", boost::program_options::value<unsigned int>(&numGames)->default_value(1),
                            "The number of games to play before exiting. 0 is infinite. Default is 1")
                    ("search-depth,sd", boost::program_options::value<unsigned int>(&searchDepth)->default_value(5),
                            "The maximum number of moves an AI will look ahead. "
                            "Only applicable to players that try to predict moves, such as ai_ab_minimax. "
                            "Larger numbers exponentially affect speed and memory use. Default is 5")
                    ("num-threads,t", boost::program_options::value<unsigned int>(&numThreads)->default_value(4),
                            "The number of worker threads an AI will use when determining moves. "
                            "Only applicable to players that use multithreading, such as ai_ab_minimax. "
                            "Default is 4");
            
            //Create the variables map
            boost::program_options::variables_map variablesMap;
            boost::program_options::store(boost::program_options::parse_command_line(
                    argc, argv, description), variablesMap);
    
            //If the user wants help
            if (variablesMap.count("help"))
            {
                std::cout << description << std::endl;
                return EXIT_SUCCESS;
            }
            //If the user wants to know the version
            if (variablesMap.count("version"))
            {
                std::cout << "Program created for final year project "
                             "\"Comparison of Reinforcement Learning Techniques to Play Othello\" "
                             "created by Benjamin S. Hollier (beh43), version 0.1" << std::endl;
                return EXIT_SUCCESS;
            }
            
            //Check the options
            boost::program_options::notify(variablesMap);
            
            //If the game should display possibly moves
            bool displayPossibleMoves = variablesMap.count("possible-moves");
            //If the game is being run in GUI mode
            bool guiMode = variablesMap.count("gui");
            //If the board should be hidden
            bool hideBoard = variablesMap.count("hide-board");
            
            //Create a game
            game::Game game;
            
            //The player objects to be created
            std::shared_ptr<game::IPlayer> players[2];
            
            //Iterate over the player types
            for (uint8_t i = 0; i < 2; ++i)
            {
                //Get the player type and convert it to lowercase
                std::string playerType = playerTypes[i];
                std::transform(playerType.begin(), playerType.end(), playerType.begin(), ::tolower);
        
                if (playerType == "human")
                {
                    if (!guiMode) {players[i].reset(new othello::cmd::HumanPlayer());}
                }
                else if (playerType == "ai_random") {players[i].reset(new othello::ai::RandomPlayer());}
                else if (playerType == "ai_ab_minimax") {players[i].reset(new othello::ai::AlphaBetaPruningPlayer(
                        game, i, searchDepth, numThreads));}
                else
                {
                    std::cerr << "Unknown player type '" << playerTypes[i] << "'" << std::endl;
                    return EXIT_FAILURE;
                }
            }
            
            //Set the players
            game.setPlayers(players[0], players[1]);
            
            //If this is being run in the command line
            if (!guiMode)
            {
                //Add a pre-turn callback
                game.addPreTurnCallback([&](const game::Game& game, const uint8_t& player)
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
                });
                
                //Add a win callback
                game.addWinCallback([&](const game::Game& game, const uint8_t& player)
                {
                    //Print the board
                    cmd::BoardPrinter::print(game.getBoard(), player, {});
                    //If it was a draw
                    if (player == 0) {std::cout << "It was a draw!" << std::endl;}
                    else
                    {
                        const auto score = game.getScore();
                        std::cout << "Player " << (int)player << " (" << (!(bool)(player - 1) ? 'X' : 'O') << ") won!" << std::endl;
                        std::cout << (unsigned int)score.first << " vs " << (unsigned int)score.second << std::endl;
                    }
                });
                
                for (unsigned int i = 0; i < numGames; ++i)
                {
                    std::cout << "Game " << i + 1 << "!" << std::endl;
                    //Play a game
                    game.playGame();
                    //Reset the game
                    game.reset();
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