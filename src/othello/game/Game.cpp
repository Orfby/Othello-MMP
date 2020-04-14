//Othello Headers:
#include <othello/game/Game.hpp>

namespace othello
{
    
    namespace game
    {
        
        ////////////////////////////////////////////////////////////////
        void Game::setPlayers(const std::shared_ptr<IPlayer>& player1,
                        const std::shared_ptr<IPlayer>& player2)
        {
            players = {player1, player2};
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::enableSwitchPlayers(bool switchPs)
        {
            switchPlayers = switchPs;
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::manualSwitchPlayers()
        {
            //Switch the players
            setPlayers(players[1], players[0]);
    
            //Flip the bool
            playersAreSwitched = !playersAreSwitched;
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::setBoard(const Board& b) {board = b;}
        
        
        ////////////////////////////////////////////////////////////////
        const Board& Game::getBoard() const {return board;}
    
    
        ////////////////////////////////////////////////////////////////
        bool Game::nextTurn()
        {
            //If either of the players are null
            if (!players[0] || !players[1]) {throw std::runtime_error("Players not set before starting the game");}
            
            //Call the pre turn callbacks
            for (const auto& callback : preTurnCallbacks)
            {
                callback(*this, board.getCurrentPlayer() + 1);
            }
    
            //Get the possible turns
            const std::vector<Move>& possibleMoves = board.getPossibleMoves();
            
            //Ask the player to make a move
            const Move* move = players[board.getCurrentPlayer()]->makeMove(
                    *this, board.getCurrentPlayer(), possibleMoves);
            //Make the move on the board
            const bool result = board.makeMove(move);
    
            //Increase the game length
            ++gameLength;
    
            //Call the post turn callbacks
            for (const auto& callback : postTurnCallbacks)
            {
                callback(*this, board.getCurrentPlayer() + 1);
            }
            
            //If the game ended
            if (result)
            {
                //Get the score
                const auto score = board.getScore();
    
                //Determine the winner
                uint8_t winner;
                if (score.first > score.second) {winner = 1;}
                else if (score.first < score.second) {winner = 2;}
                else {winner = 0;} //winner 0 means a draw
                
                //Record the stats
                if (stats)
                {
                    if (!playersAreSwitched) {stats->gameEnd(score, gameLength);}
                    else {stats->gameEnd({score.second, score.first}, gameLength);}
                }
                
                //Call the player's game finished function
                players[0]->gameFinished(*this, 0);
                players[1]->gameFinished(*this, 1);
                
                //Call the callbacks
                for (const auto& callback : winCallbacks) {callback(*this, winner);}
                
                //If the players should be switched
                if (switchPlayers) {manualSwitchPlayers();}
            }
            
            //Return the result
            return result;
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::playGame() {while (!nextTurn()) {}}
    
    
        ////////////////////////////////////////////////////////////////
        const std::pair<uint8_t, uint8_t>& Game::getScore() const {return board.getScore();}
    
    
        ////////////////////////////////////////////////////////////////
        bool Game::isOver() const {return board.isOver();}
    
    
        ////////////////////////////////////////////////////////////////
        void Game::addPreTurnCallback(const callback_func_t& callback)
        {
            preTurnCallbacks.emplace_back(callback);
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::addPostTurnCallback(const callback_func_t& callback)
        {
            postTurnCallbacks.emplace_back(callback);
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::addWinCallback(const callback_func_t& callback)
        {
            winCallbacks.emplace_back(callback);
        }
    
    
        ////////////////////////////////////////////////////////////////
        void Game::enableStatistics(const std::string& outFilePath, const std::string& infoString)
        {
            //Create the statistics manager
            stats.emplace(outFilePath, infoString);
        }
    
    
        ////////////////////////////////////////////////////////////////
        boost::optional<stats::StatisticsManager>& Game::getStatsManager() {return stats;}
    
        
        ////////////////////////////////////////////////////////////////
        void Game::reset()
        {
            board.reset();
            gameLength = 0;
        }
        
    }
    
}