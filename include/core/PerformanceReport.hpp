#ifndef _PERFORMANCE_REPORT_HPP_
#define _PERFORMANCE_REPORT_HPP_

#include <vector>
#include <string>
#include "core/Position.hpp"
#include "core/Backtester.hpp"


namespace bt{

/*---------------------------------------- class PerformanceReport ---------------------------------------*/
/*
Renseigne sur les performances a la fin de la simulation et fournit un fichier dans le dossier "data" pour
l'analyse des resultats.
*/

class PerformanceReport{
    private:
        const std::vector<Position>& closedPositions;

        double totalPnl{0.0};
        int totalTrades{0};
        int winningTrades{0};
        int losingTrades{0};
        double winRate{0.0};
        double pnlRealized{0.0};

        public:
            PerformanceReport() = default;
            PerformanceReport(const std::vector<Position>& positions);
            
            
            int generateReport(const std::string &filePath, const Backtester &b);
            void CalculatePnl();
};

} //end namespace bt

#endif