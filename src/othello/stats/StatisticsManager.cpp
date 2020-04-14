//Standard C++:
#include <iostream>
#include <iomanip>
//Othello headers:
#include <othello/stats/StatisticsManager.hpp>


namespace othello
{
    
    namespace stats
    {
    
        ////////////////////////////////////////////////////////////////
        StatisticsManager::StatisticsManager(const std::string& outFilePath, const std::string& infoString)
        {
            //Open the file and delete anything that's already there
            outFile.open(outFilePath, std::ios::trunc);
            //Output the header
            outFile << infoString << std::endl;
            outFile << "CSV START" << std::endl;
            outFile << "batch,p1AvgWins,p1AvgScore,p2AvgWins,p2AvgScore,avgGameLen,elapsed" << std::endl;
        }
    
    
        ////////////////////////////////////////////////////////////////
        void StatisticsManager::gameEnd(const std::pair<uint8_t, uint8_t>& score, const unsigned int& gameLength)
        {
            //Increase the number of games
            ++numGames;
            
            //Increase the game length
            totalGameLength += gameLength;
            
            //Increase the score
            totalScore.first += score.first;
            totalScore.second += score.second;
            
            //Increase the number of wins for the winner
            if      (score.first > score.second) {++numWins.first;}
            else if (score.second > score.first) {++numWins.second;}
        }
    
    
        ////////////////////////////////////////////////////////////////
        void StatisticsManager::output()
        {
            //Get the time now
            auto end = std::chrono::system_clock::now();
    
            //Calculate the elapsed time
            std::chrono::duration<double> elapsed = end - start;
            
            //Calculate the average wins
            std::pair<double, double> avgWins = {
                    (double)numWins.first / numGames,
                    (double)numWins.second / numGames
            };
            
            //Calculate the average score
            std::pair<double, double> avgScore = {
                    (double)totalScore.first / numGames,
                    (double)totalScore.second / numGames
            };
            
            //Calculate the average game length
            double avgGameLen = (double)totalGameLength / numGames;
            
            //Output to the csv
            outFile << batchCounter << ",";
            outFile << avgWins.first  << "," << avgScore.first  << ",";
            outFile << avgWins.second << "," << avgScore.second << ",";
            outFile << avgGameLen << "," << elapsed.count() << std::endl;
            
            //Output to stdout
            std::cout << "======================" << std::endl;
            std::cout << "Statistics of " << numGames << " games:" << std::endl;
            std::cout << "Win Rate:        ";
            std::cout << std::setprecision(6) << avgWins.first * 100 << " vs ";
            std::cout << std::setprecision(6) << avgWins.second * 100 << std::endl;
            std::cout << "Avg Score:       ";
            std::cout << std::setprecision(6) << avgScore.first << " vs ";
            std::cout << std::setprecision(6) << avgScore.second << std::endl;
            std::cout << "Avg Game Length: ";
            std::cout << std::setprecision(6) << avgGameLen << std::endl;
            std::cout << "Elapsed Time: ";
            std::cout << std::setprecision(6) << elapsed.count() << std::endl;
            std::cout << "======================" << std::endl;
        }
    
    
        ////////////////////////////////////////////////////////////////
        void StatisticsManager::reset()
        {
            //Reset the stats
            numGames = 0;
            numWins = {0, 0};
            totalScore = {0, 0};
            totalGameLength = 0;
            start = std::chrono::system_clock::now();
        }
        
        
        ////////////////////////////////////////////////////////////////
        void StatisticsManager::nextBatch()
        {
            //Reset the stats
            reset();
            
            //Increase the batch counter
            ++batchCounter;
        }
        
    }
    
}