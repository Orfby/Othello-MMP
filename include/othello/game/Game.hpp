#ifndef OTHELLO_GAME_GAME_HPP
#define OTHELLO_GAME_GAME_HPP

//Standard C++:
#include <cstdint>
#include <array>
#include <memory>
#include <functional>
//Othello Headers:
#include <othello/game/Board.hpp>
#include <othello/game/IPlayer.hpp>


namespace othello
{
    
    namespace game
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class Game
        ///
        /// \brief Class that manages a single Othello game
        ///
        ////////////////////////////////////////////////////////////////
        class Game
        {
            public:
                
                ////////////////////////////////////////////////////////////////
                /// \brief Convenience typedef for a pre turn callback function
                ///
                /// The first argument is a const reference to the game the turn
                /// is happening in. The second is the player's number (either 1
                /// or 2)
                ///
                ////////////////////////////////////////////////////////////////
                typedef std::function<void(const Game&, const uint8_t&)> callback_func_t;
                
                
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \brief The game board
                ///
                ////////////////////////////////////////////////////////////////
                Board board;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The two players
                ///
                ////////////////////////////////////////////////////////////////
                std::array<std::shared_ptr<IPlayer>, 2> players = {nullptr, nullptr};
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of callbacks that are called before a new
                ///        turn
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<callback_func_t> preTurnCallbacks;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of callbacks that are called after a new
                ///        turn
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<callback_func_t> postTurnCallbacks;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief A vector of callbacks that are called when a player
                ///        wins
                ///
                ////////////////////////////////////////////////////////////////
                std::vector<callback_func_t> winCallbacks;
                
                
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to set the player pointers
                ///
                /// \param player1 A shared pointer to an IPlayer that acts as
                ///                player 1 (blacks, starts first)
                /// \param player2 A shared pointer to an IPlayer that acts as
                ///                player 2 (whites, starts second)
                ///
                ////////////////////////////////////////////////////////////////
                void setPlayers(const std::shared_ptr<IPlayer>& player1,
                        const std::shared_ptr<IPlayer>& player2);
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get a const reference to the game board
                ///
                ////////////////////////////////////////////////////////////////
                const Board& getBoard() const;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to play the next turn
                ///
                /// \note The game is not automatically reset after a game ends
                ///
                /// \return Whether the game ended in this turn
                ///
                ////////////////////////////////////////////////////////////////
                bool nextTurn();
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Convenience function to play a game to completion
                ///
                /// \note The game is not automatically reset after a game ends
                ///
                /// Equivalent to `while (!game.nextTurn()) {}`
                ///
                ////////////////////////////////////////////////////////////////
                void playGame();
        
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to get the board's current score (number of
                ///        disks for each player)
                ///
                /// \return A const reference pair where the first value is
                ///         player 1's score and the second is player 2's score
                ///
                ////////////////////////////////////////////////////////////////
                const std::pair<uint8_t, uint8_t>& getScore() const;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to determine if the game on the board is
                ///        over
                ///
                ////////////////////////////////////////////////////////////////
                bool isOver() const;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief Function to add a callback function for before
                ///        `IPlayer::makeMove` is called
                ///
                /// \note This function isn't called for players that can't make
                ///       a turn and have to pass
                ///
                ////////////////////////////////////////////////////////////////
                void addPreTurnCallback(const callback_func_t& callback);
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to add a callback function for after
                ///        `Board::makeMove` is called
                ///
                /// \note This function isn't called for players that can't make
                ///       a turn and have to pass
                /// \note This function is called before any win callbacks are
                ///       called
                ///
                ////////////////////////////////////////////////////////////////
                void addPostTurnCallback(const callback_func_t& callback);
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to add a callback function for when a player
                ///        wins the game (or if there is a draw)
                ///
                /// \note The player given to the callback is the winner, where
                ///       1 == player 1, 2 == player2, 0 == draw
                ///
                ////////////////////////////////////////////////////////////////
                void addWinCallback(const callback_func_t& callback);
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to reset the game
                ///
                ////////////////////////////////////////////////////////////////
                void reset();
                
        };
        
    }
    
}

#endif //OTHELLO_GAME_GAME_HPP
