#include "core/PerformanceReport.hpp"
#include <iostream>
#include <fstream>



namespace bt{

PerformanceReport::PerformanceReport(const std::vector<Position>& positions) : closedPositions(positions) {}



/*--------------------------------- CalculatePnl -----------------------------------*/
/*
Additionne les profits/pertes de tout les positions pour obtenir le resultat final.
*/

void PerformanceReport::CalculatePnl(){
    if(closedPositions.size() > 0){
        for(auto it= closedPositions.begin(); it!= closedPositions.end(); it++){
            pnlRealized += it->getPnlRealized();
        }
    }
}



/*--------------------------------- generateReport -----------------------------------*/
/*
Cree un fichier txt dans le dossier data pour analyser les performances. Methode appelee par 
le backtester en fin de simulation.
*/

int PerformanceReport::generateReport(const std::string &filePath, const Backtester &b){
    
    std::ofstream file(filePath); 
    if(!file.is_open()){
        std::cerr << "Error on opening report file\n";
        return 1;   
    }

    winningTrades = b.getWinningTrades();
    losingTrades = b.getLosingTrades();
    CalculatePnl();
    totalTrades = losingTrades + winningTrades;
    if(totalTrades != 0)
        winRate = (static_cast<double>(winningTrades)/totalTrades) * 100;
    else 
        winRate = 0.0;

    file << "------- Report of simulation on : " << b.getAsset() << " ---------\n\n" <<
    "Strategy used : " << b.getStrategyInUse() << "\n" <<
    "Numbers of Winning trades : " << winningTrades << "\n" <<
    "Numbers of Losing trades : " << losingTrades << "\n" <<
    "Win Rate : " << winRate << "%\n" <<
    "PnlRealized : " << pnlRealized << std::endl;

    file << "\n\n---------- Portfolio ---------------\n";
    b.printPortfolio(file);

    file << "\n\n---------- Historique des positions ---------------\n";
    if(closedPositions.size() > 0){
        for(auto it= closedPositions.begin(); it!= closedPositions.end(); it++){
            file << *it << "\n\n";
        }
    }

    return 0;
}


}