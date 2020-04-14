#ifndef OTHELLO_STATS_STATISTICSMANAGER_HPP
#define OTHELLO_STATS_STATISTICSMANAGER_HPP

//Standard C++:
#include <fstream>
#include <chrono>

namespace othello
{
    
    namespace stats
    {
    
        ////////////////////////////////////////////////////////////////
        /// \class StatisticsManager
        ///
        /// \brief Class to collect and calculate statistics about the
        ///        program
        ///
        ////////////////////////////////////////////////////////////////
        class StatisticsManager
        {
            private:
        
                ////////////////////////////////////////////////////////////////
                /// \brief The output CSV file
                ///
                ////////////////////////////////////////////////////////////////
                std::ofstream outFile;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The counter for the number of game batches
                ///
                ////////////////////////////////////////////////////////////////
                unsigned int batchCounter = 1;
                
                
                ////////////////////////////////////////////////////////////////
                /// \brief The number of games played
                ///
                ////////////////////////////////////////////////////////////////
                unsigned long numGames = 0;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The number of wins for each player
                ///
                ////////////////////////////////////////////////////////////////
                std::pair<unsigned long, unsigned long> numWins = {0, 0};
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The total score for each player
                ///
                ////////////////////////////////////////////////////////////////
                std::pair<unsigned long, unsigned long> totalScore = {0, 0};
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The total game length
                ///
                ////////////////////////////////////////////////////////////////
                unsigned long totalGameLength = 0;
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief The time point of when the batch started
                ///
                ////////////////////////////////////////////////////////////////
                std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
                
            
            public:
        
                ////////////////////////////////////////////////////////////////
                /// \brief Class constructor
                ///
                /// \param outFilePath The path to the output csv file
                /// \param infoString A string containing information about
                ///        program arguments/parameters to prepend to the output
                ///        file
                ///
                ////////////////////////////////////////////////////////////////
                explicit StatisticsManager(const std::string& outFilePath, const std::string& infoString);
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function that is called when a game ends
                ///
                ////////////////////////////////////////////////////////////////
                void gameEnd(const std::pair<uint8_t, uint8_t>& score, const unsigned int& gameLength);
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to output the collected statistics
                ///
                ////////////////////////////////////////////////////////////////
                void output();
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to reset the current statistics
                ///
                ////////////////////////////////////////////////////////////////
                void reset();
        
        
                ////////////////////////////////////////////////////////////////
                /// \brief Function to go to the next batch. Calls `reset()`
                ///
                ////////////////////////////////////////////////////////////////
                void nextBatch();
                
                
        };
        
    }
    
}

#endif //OTHELLO_STATS_STATISTICSMANAGER_HPP
