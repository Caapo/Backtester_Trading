#include <iostream>
#include <filesystem>
#include <format>
#include "utils.hpp"
#include "core/Backtester.hpp"
#include "strategies/IStrategy.hpp"

using bt::Backtester;
using bt::verifCin;
using bt::IStrategy;



/*-------------- Attributs statiques --------------*/

int IStrategy::strategiesCount{0}; 






/*-------------------------------------------------- MAIN ---------------------------------------------------------*/

int main(){

    /* -------------------------- Preparation de la simulation -------------------------- */

    std::cout << "Bienvenue dans la simulation de trading pour le backtesting\n";
    
    double capital_{-1.0};
    double riskP_{-1.0}; 
    double RRR{-1.0};
    double leverage{-1.0};

    //Renseignement du capital
    while(capital_ <= 0){
        std::cout << "Renseignez votre capital (>0): ";
        std::cin >> capital_;
        verifCin(capital_);
    }

    //Renseignement du risque du compte
    while(riskP_ > 100 || riskP_ <= 0){
        std::cout << "Renseignez votre risque par rapport au capital (% && >0) : ";
        std::cin >> riskP_;
        verifCin(riskP_);
    }
    
    //Renseignement du Ratio Risk/Reward
    while(RRR <= 0){
        std::cout << "Renseignez votre ratio Risk/Reward (>0): ";
        std::cin >> RRR;
        verifCin(RRR);
    }




    /* -------------------------- Lancement du Backtester -------------------------- */


    /*Construction du backtester*/    
    Backtester backtester(capital_, riskP_, RRR, leverage);
    
    /*Lancement de la simulation*/
    try{backtester.run();}
    catch(const std::exception &e){
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    





    return 0;
}