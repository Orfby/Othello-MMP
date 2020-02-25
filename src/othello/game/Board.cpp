//Standard C++:
#include <functional>
#include <iostream>
//Othello headers:
#include <othello/game/Board.hpp>


namespace othello
{
    
    namespace game
    {
        
        ////////////////////////////////////////////////////////////////
        /// \brief Function that recalculates all the moves each turn
        ///        by searching in 8 directions from each unclaimed tile
        ///
        ////////////////////////////////////////////////////////////////
        void Board::calculatePossibleMoves(const Move)
        {
            //Reset the possible moves for this player
            possibleMoves[currentPlayer].clear();
            
            //Create a vector of flipped disk tiles
            std::vector<TilePosition> flippedDiskLocations;
        
            //Lambda for searching from a given tile in a given direction for a valid move
            auto searchInDirection = [&](const TilePosition& pos, int8_t xDir, int8_t yDir)
            {
                //Clear the vector
                flippedDiskLocations.clear();
            
                //Start at the neighbouring tile in the given direction
                int16_t x = pos.x + xDir;
                int16_t y = pos.y + yDir;
                //While the tile isn't off the board
                while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
                {
                    //If the tile is claimed by the other player
                    if (grid[y][x].isClaimed && grid[y][x].claimant != currentPlayer)
                    {
                        //Add the tile as a flipped disk
                        flippedDiskLocations.emplace_back(TilePosition{(uint8_t)x, (uint8_t)y});
                    }
                        //If it's claimed by the current player
                    else if (grid[y][x].isClaimed && grid[y][x].claimant == currentPlayer)
                    {
                        //If we haven't flipped any disks to get here (aka this tile is next to the start tile), just exit
                        if (flippedDiskLocations.empty()) {return;}
                        else
                        {
                            //Add the move
                            possibleMoves[currentPlayer].emplace_back(Move(currentPlayer, pos, flippedDiskLocations));
                            //Exit
                            return;
                        }
                    }
                        //This tile is unclaimed so there can't be an uninterrupted line, exit
                    else {return;}
                
                    //Move to the next tile
                    x += xDir;
                    y += yDir;
                }
            };
        
            //Iterate over the tiles in the board
            for (uint8_t y = 0; y < BOARD_SIZE; ++y) {for (uint8_t x = 0; x < BOARD_SIZE; ++x)
            {
                //If it hasn't been claimed
                if (!grid[y][x].isClaimed)
                {
                    //Check for valid moves in all 8 directions
                    searchInDirection({x, y}, 1, 0); //right
                    searchInDirection({x, y}, -1, 0); //left
                    searchInDirection({x, y}, 0, 1); //down
                    searchInDirection({x, y}, 0, -1); //up
                    searchInDirection({x, y}, 1, 1); //down-right
                    searchInDirection({x, y}, -1, -1); //up-left
                    searchInDirection({x, y}, 1, -1); //up-right
                    searchInDirection({x, y}, -1, 1); //down-left
                }
            }}
            
            //Look for moves to merge
            //Iterate over the possible moves
            for (std::size_t i = 0; i < possibleMoves[currentPlayer].size(); ++i)
            {
                //Iterate over the other moves
                for (std::size_t j = i + 1; j < possibleMoves[currentPlayer].size(); ++j)
                {
                    //If the two moves are for placing a disk in the same place
                    if ((possibleMoves[currentPlayer][i].diskPosition.x ==
                        possibleMoves[currentPlayer][j].diskPosition.x) &&
                        (possibleMoves[currentPlayer][i].diskPosition.y ==
                        possibleMoves[currentPlayer][j].diskPosition.y))
                    {
                        //Insert the other move's flipped disks to this one
                        possibleMoves[currentPlayer][i].flippedDiskPositions.insert(
                                possibleMoves[currentPlayer][i].flippedDiskPositions.end(),
                                possibleMoves[currentPlayer][j].flippedDiskPositions.begin(),
                                possibleMoves[currentPlayer][j].flippedDiskPositions.end());
                        //Remove the other possible move from the possible moves vector
                        possibleMoves[currentPlayer].erase(possibleMoves[currentPlayer].begin() + j);
                        //Decrease j
                        --j;
                    }
                }
            }
        }
    
    
        ////////////////////////////////////////////////////////////////
        /// \brief Function that modifies the possibleMoves vectors by
        ///        adding or removing possible moves based on each newly
        ///        placed disk
        ///
        ////////////////////////////////////////////////////////////////
        //My IDE is suggesting I make previousMove a reference, but it *must* be
        //copied by val because we're modifying the vector it is in
        //(possibleMoves) so the move might be modified/removed from the vector,
        //or when the vector reallocs the pointer/reference will be invalidated
        /*void Board::calculatePossibleMoves(const Move previousMove)
        {
            //Create a temporary vector for storing flipped disks
            std::vector<TilePosition> flippedDisksTmp;
    
            //A lambda that iterates in a given direction and calls func on each tile.
            //If func returns false it exits the function early
            auto iterateInDirection = [&](const TilePosition& start, int8_t xDir, int8_t yDir,
                                          const std::function<bool(const TilePosition&, const Tile&)>& func)
            {
                //Clear the temp flipped disks vector
                flippedDisksTmp.clear();
                //Start at the neighbouring tile in the given direction
                int16_t x = ((int16_t)start.x) + xDir;
                int16_t y = ((int16_t)start.y) + yDir;
                //While the tile isn't off the board
                while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
                {
                    if (!func({(uint8_t) x, (uint8_t) y}, grid[y][x])) {return;}
                    x += xDir;
                    y += yDir;
                }
            };
    
            //A lambda that calls iterateInDirection on all 8 directions
            auto iterateInAllDirections = [&](const TilePosition& start,
                                              const std::function<bool(const TilePosition&, const Tile&)>& func)
            {
                iterateInDirection(start, 1, 0, func); //right
                iterateInDirection(start, -1, 0, func); //left
                iterateInDirection(start, 0, 1, func); //down
                iterateInDirection(start, 0, -1, func); //up
                iterateInDirection(start, 1, 1, func); //down-right
                iterateInDirection(start, -1, -1, func); //up-left
                iterateInDirection(start, 1, -1, func); //up-right
                iterateInDirection(start, -1, 1, func); //down-left
            };
        
            //Iterate over the players
            for (uint8_t player = 0; player < 2; ++player)
            {
                {
                    //Look for moves around the placed and flipped disks
                    auto lookForMoves = [&](const TilePosition& pos, const Tile& tile)
                    {
                        //If the tile isn't claimed and so could have a disk placed
                        if (!tile.isClaimed)
                        {
                            //Make sure the move doesn't already exist
                            for (const auto& move : possibleMoves[player])
                            {
                                //If it matches exit
                                if (move.diskPosition.x == pos.x &&
                                    move.diskPosition.y == pos.y) {return false;}
                            }
            
                            //Add the move
                            possibleMoves[player].emplace_back(
                                    Move(player, pos));
            
                            //Exit
                            return false;
                        }
                        //If the tile is claimed by the player, exit
                        //(because you can't flip your own disk)
                        else if (tile.claimant == player) {return false;}
                        return true;
                    };
    
                    iterateInAllDirections(previousMove.diskPosition, lookForMoves);
                    for (const auto& flippedDisk : previousMove.flippedDiskPositions)
                    {
                        iterateInAllDirections(flippedDisk, lookForMoves);
                    }
                }
                
                //Iterate over the possible moves
                for (std::size_t i = 0; i < possibleMoves[player].size(); ++i)
                {
                    //Clear the move's flipped disks
                    possibleMoves[player][i].flippedDiskPositions.clear();
                    
                    //A lambda to check for the tile in a direction for a possible move
                    auto checkMove = [&](const TilePosition& pos, const Tile& tile) -> bool
                    {
                        //If the tile is unclaimed, there can't be an uninterrupted line, exit
                        if (!tile.isClaimed) {return false;}
                        //If it's claimed by the player
                        else if (tile.claimant == player)
                        {
                            //If some disks have been flipped to get to this tile
                            if (!flippedDisksTmp.empty())
                            {
                                //Add the flipped disks to the move
                                possibleMoves[player][i].flippedDiskPositions.insert(
                                        possibleMoves[player][i].flippedDiskPositions.end(),
                                        flippedDisksTmp.begin(), flippedDisksTmp.end());
                            }
                            return false;
                        }
                        //If the tile is claimed by the other player
                        else
                        {
                            //Add the tile as a flipped disk
                            flippedDisksTmp.emplace_back(pos);
                            return true;
                        }
                    };
        
                    iterateInAllDirections(possibleMoves[player][i].diskPosition, checkMove);
        
                    //If the possible move doesn't flip any disks anymore
                    if (possibleMoves[player][i].flippedDiskPositions.empty())
                    {
                        //Remove the move
                        possibleMoves[player].erase(possibleMoves[player].begin() + i);
                        //Decrease i
                        --i;
                    }
                }
            }
        }*/
        
        
        ////////////////////////////////////////////////////////////////
        Board::Board() {reset();}
        
        
        ////////////////////////////////////////////////////////////////
        const Tile& Board::getTile(const TilePosition& pos) const {return grid[pos.y][pos.x];}
    
    
        ////////////////////////////////////////////////////////////////
        uint8_t Board::getCurrentPlayer() const {return currentPlayer;}
    
    
        ////////////////////////////////////////////////////////////////
        const std::vector<Move>& Board::getPossibleMoves() const {return possibleMoves[getCurrentPlayer()];}
    
    
        ////////////////////////////////////////////////////////////////
        bool Board::makeMove(const Move* const move)
        {
            //Make sure the move pointer is within the vector
            //This is legal in C++0x and above according to the standard, which states "The elements of a
            //vector are stored contiguously, meaning that if v is a vector<T, Allocator> where T is some type other
            //than bool, then it obeys the identity &v[n] == &v[0] + n for all 0 <= n < v.size()"
            //(see 23.2.6 in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2798.pdf)
            if (move < &getPossibleMoves().front() || move > &getPossibleMoves().back())
            {
                //Throw an exception
                throw std::invalid_argument("Move pointer given to 'Board::makeMove' doesn't "
                                            "point to an element within getPossibleMoves() vector");
            }
    
            //Place a disk where the move says
            grid[move->diskPosition.y][move->diskPosition.x].isClaimed = true;
            grid[move->diskPosition.y][move->diskPosition.x].claimant = getCurrentPlayer();
    
            //Iterate over the disks that are flipped by the move
            for (const TilePosition& tile : move->flippedDiskPositions)
            {
                //Set the claimant to the current player
                grid[tile.y][tile.x].claimant = currentPlayer;
            }
            
            //Update the score
            {
                //Increase the player's score by the number of flipped
                //disks plus 1 (for the one they placed) and decrease
                //the other player's score by the number of flipped disks
                std::size_t numFlipped = move->flippedDiskPositions.size();
                if (getCurrentPlayer() == 0)
                {
                    score.first += numFlipped + 1;
                    score.second -= numFlipped;
                }
                else
                {
                    score.second += numFlipped + 1;
                    score.first -= numFlipped;
                }
            }
            
            //Go to the next player
            currentPlayer = !currentPlayer;
            if (currentPlayer) {currentPlayer = 1;}
            
            //Calculate the new possible moves for each player
            calculatePossibleMoves(*move);
            
            //If the player can't make a turn
            if (getPossibleMoves().empty())
            {
                //Go to the next player
                currentPlayer = !currentPlayer;
                if (currentPlayer) {currentPlayer = 1;}
    
                //Calculate the new possible moves for each player
                calculatePossibleMoves(*move);
                
                //If they also can't make a turn, the game is over
                if (getPossibleMoves().empty())
                {
                    finished = true;
                    return true;
                }
            }
            
            //Return the game hasn't ended
            return false;
        }
    
    
        ////////////////////////////////////////////////////////////////
        std::pair<uint8_t, uint8_t> Board::getScore() const {return score;}
    
    
        ////////////////////////////////////////////////////////////////
        bool Board::isOver() const {return finished;}
        
        
        ////////////////////////////////////////////////////////////////
        void Board::reset()
        {
            //Reset the current player
            currentPlayer = 0;
            
            //Reset the score
            score.first = 2;
            score.second = 2;
            
            //The game is no longer finished
            finished = false;
            
            //Empty the board
            for (uint8_t y = 0; y < BOARD_SIZE; ++y) {for (uint8_t x = 0; x < BOARD_SIZE; ++x)
                {
                    grid[y][x].isClaimed = false;
                }}
    
            //Set the board's initial state
            //Player 1, top right
            grid[(BOARD_SIZE / 2) - 1][BOARD_SIZE / 2].isClaimed = true;
            grid[(BOARD_SIZE / 2) - 1][BOARD_SIZE / 2].claimant = false;
            calculatePossibleMoves(Move(0, {(BOARD_SIZE / 2) - 1, BOARD_SIZE / 2}));
            //Bottom left
            grid[BOARD_SIZE / 2][(BOARD_SIZE / 2) - 1].isClaimed = true;
            grid[BOARD_SIZE / 2][(BOARD_SIZE / 2) - 1].claimant = false;
            calculatePossibleMoves(Move(0, {BOARD_SIZE / 2, (BOARD_SIZE / 2) - 1}));
    
            //Player 2, top left
            grid[(BOARD_SIZE / 2) - 1][(BOARD_SIZE / 2) - 1].isClaimed = true;
            grid[(BOARD_SIZE / 2) - 1][(BOARD_SIZE / 2) - 1].claimant = true;
            calculatePossibleMoves(Move(1, {(BOARD_SIZE / 2) - 1, (BOARD_SIZE / 2) - 1}));
            //Bottom right
            grid[BOARD_SIZE / 2][BOARD_SIZE / 2].isClaimed = true;
            grid[BOARD_SIZE / 2][BOARD_SIZE / 2].claimant = true;
            calculatePossibleMoves(Move(1, {BOARD_SIZE / 2, BOARD_SIZE / 2}));
        }
    
    }
    
}