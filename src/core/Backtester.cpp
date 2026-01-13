#include "core/Candle.hpp"
#include "core/Order.hpp"
#include "core/Position.hpp"
#include "core/Backtester.hpp"
#include "core/PerformanceReport.hpp"
#include "utils.hpp"
#include <iostream>
#include <thread>
#include <chrono>




namespace bt{


// Backtester::Backtester(const std::string &path, const std::string &asset, double capital_, double riskP_, double RRR, 
// double leverage) : market(path, asset), portfolio(capital_, riskP_, RRR, leverage){
//     //initialisation des actifs disponibles
//     assets.emplace_back("FOREX", "EUR/USD", "data/EURUSD.csv", 5);
//     assets.emplace_back("FOREX", "GBP/USD", "data/GBPUSD.csv", 5);
//     assets.emplace_back("FOREX", "USD/JPY", "data/USDJPY.csv", 3);
//     assets.emplace_back("FOREX", "XAU/USD", "data/XAUUSD.csv", 2);

//     //initialisations des strategies
//     strategies.push_back(std::make_unique<RandomS>("RandomS"));
//     strategies.push_back(std::make_unique<SMA>("SMA"));

// }



/*------------------------------- Backtester::Backtester --------------------------------*/
/*But : Construit le Backtester*/

Backtester::Backtester(double capital_, double riskP_, double RRR, double leverage) 
: portfolio(capital_, riskP_, RRR, leverage){

    //initialisation des actifs disponibles
    assets.emplace_back("FOREX", "EUR/USD", "data/EURUSD1440.csv", 5, 30);
    assets.emplace_back("FOREX", "GBP/USD", "data/GBPUSD1440.csv", 5, 30);
    assets.emplace_back("FOREX", "USD/JPY", "data/USDJPY1440.csv", 3, 30);
    assets.emplace_back("COMMODITE", "XAU/USD", "data/XAUUSD1440.csv", 2, 15);
    assets.emplace_back("CRYPTO", "BTC/USD", "data/BTCUSD1440.csv", 2, 5);

    //initialisations des strategies
    strategies.push_back(std::make_unique<RandomS>("RandomS"));
    strategies.push_back(std::make_unique<SMA>("SMA"));
    strategies.push_back(std::make_unique<Bollinger>("Bollinger"));
    strategies.push_back(std::make_unique<Momentum>("Momentum"));

}


/*------------------------------- Backtester::printA --------------------------------*/
/*But : Affiche la liste des actifs disponibles*/

void Backtester::printA(){
    for(size_t i=0; i<assets.size(); i++){
        std::cout << assets[i].getName() << " : " << i
        << " (Levier Max : " << assets[i].getMaxLeverage() << ")" << "\n"; 
    }
    std::cout << std::endl;
}


/*------------------------------- Backtester::selectA --------------------------------*/
/*But : Pouvoir selectionner un actif pour la simulation*/

size_t Backtester::selectA(){
    
    std::cout << "\nActifs disponibles :\n";
    printA();

    std::cout << "Selectionner un actif (id) : ";
    size_t id;
    std::cin >> id;
    
    while(std::cin.fail() || id >= assets.size()){
        std::cout << "Format non attendu/Actif inconnu - Essayez a nouveau : ";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cin >> id;
    }

    std::cout << "Vous avez selectionne : " << assets[id].getName() << std::endl;
    std::cin.ignore(1000, '\n');
    return id;
}


/*------------------------------- Backtester::printS --------------------------------*/
/*But : Affiche la liste des strategies disponibles*/

void Backtester::printS(){
    for(size_t i=0; i<strategies.size(); i++){
        std::cout << strategies[i]->getName() << " (id="  << i << ")\n"; 
    }
    std::cout << std::endl;
}


/*------------------------------- Backtester::selectS --------------------------------*/
/*But : Pouvoir selectionner une strategie pour la simulation*/

size_t Backtester::selectS(){
    
    std::cout << "\nStrategies disponibles :\n";
    printS();

    std::cout << "Selectionner une strategie (id) : ";
    size_t id;
    std::cin >> id;
    
    while(std::cin.fail() || id >= strategies.size()){
        std::cout << "Format non attendu/Strategie inconnue - Essayez a nouveau : : ";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cin >> id;
    }

    std::cout << "Vous avez selectionne : " << strategies[id]->getName() << std::endl;
    std::cin.ignore(1000, '\n');
    return id;
}


/*------------------------------- Backtester::listenCommands --------------------------------*/
/*But : Pouvoir taper des commandes au cours de la simulation afin de gerer ses positions*/

void Backtester::listenCommands(){

    std::string cmd;

    while(stopRequest == false){
        std::cin >> cmd;

        if(cmd == "quit" || cmd == "stop"){
            std::cout << "---------- [COMMANDS] ----------\n";
            std::cout << "Simulation ending...\n";
            stopRequest = true;
            break;
        }

        if(cmd == "close"){
            waitInput = true;
            int id;
            std::cout << "---------- [COMMANDS] ----------\n";
            std::cout << "ID OF THE POSITION YOU WANT TO CLOSE : ";
            std::cin >> id;
            std::cin.ignore(100, '\n');
            idClosePos = id;

            waitInput = false;
        }

        if(cmd == "closeall"){
            std::cout << "---------- [COMMANDS] ----------\n";
            std::cout << "Closing all positions...\n";
            closeAll = 1;
        }

        if(cmd == "printid"){
            waitInput = true;
            int id;
            std::cout << "---------- [COMMANDS] ----------\n";
            std::cout << "ID OF A POSITION TO GET ITS INFORMATIONS : ";
            std::cin >> id;
            std::cin.ignore(100, '\n');
            idShowPos = id;
            
            waitInput = false;
        }

        if(cmd == "print"){
            std::cout << "---------- [COMMANDS] ----------\n";
            print = 1;
            // waitInput = true;
            // portfolio.showAllPos();
            // waitInput = false;
        }                

        if(cmd == "autotrade"){
            waitInput = true;
            std::cout << "---------- [COMMANDS] ----------\n";
            int tmp;
            std::cout << "Select auto trade mod : [1/0] (Enable, Disable)\n";

            if(verifCin(tmp, 0, 1)){
                autoTrade = tmp;
                if(autoTrade == 1)
                    std::cout << "Autotrade enabled\n";
                else
                    std::cout << "Autotrade disabled\n";
            }
            else{
                std::cout << "Invalid input\n";
            }

            waitInput = false;
        }


        if(!autoTrade && cmd=="1"){
            executeTrade = 1;
            waitInput = false;
        }
        if(!autoTrade && cmd=="0"){
            executeTrade = 0;
            waitInput = false;
        }

        if(cmd == "speed"){
            waitInput = true;
            std::cout << "---------- [COMMANDS] ----------\n";
            int s;
            std::cout << "Enter number of seconds between the candles\n" 
                      << "[From 0(Instant) to 15(Very Slow)]: ";

            if(verifCin(s, 0, 15)){
                speedLevel = s;
                std::cout << "Speed updated to " << speedLevel << "\n";
            }
            else{
                std::cout << "Invalid input\n";
            }

            waitInput = false;
        }

        /*Systeme de pause durant la simulation pour prendre le temps de voir ce qu'il se passe*/
        if(cmd == "pause"){
            std::cout << "Simulation paused, please enter 'resume' to get back in\n";
            waitInput = true;
        }

        if(cmd == "resume"){
            waitInput = false;
        }



    }
}




/*------------------------------- Backtester::run --------------------------------*/
/*But : Chef d'orchestre de la simulation*/

void Backtester::run(){

    /*Selection de l'actif sur lequel simuler*/
    Asset a = assets[selectA()];
    market.loadCSV(a.getPath(), a.getName());

    //Renseignement du levier du compte (Effet de levier - varie selon l'actif)
    double leverage_{-1.0};
    while(leverage_ <= 0 || leverage_ > a.getMaxLeverage()){
        std::cout << "\nRenseignez votre levier (> 0 et < Levier Max) : ";
        std::cin >> leverage_;
        verifCin(leverage_);
    }

    portfolio.setLeverage(leverage_);
    

    /*Selection de la strategie a utiliser*/
    size_t idS = selectS();
    IStrategy &strat = *strategies[idS];
    strategyInUse = strat.getName();

    strat.onStart();

    std::cout << "\n[BACKTESTER] STRATING SIMULATION with " << strat.getName() << "...\n";
    /*Listener pour les commandes utilisateur*/
    std::thread listener(&Backtester::listenCommands, this);

    std::cout << "\n-----------------------------------------------------------" << std::endl;
    std::cout << "\n\n<!> Start in few seconds <!>\n" 
              << "<!> Please take a look to the following commands <!>\n" << std::endl;

    std::cout << "--- [COMMANDS] User commands list (To use in the terminal) ---\n\n";

    std::cout << "quit/stop : Stop the simulation and generate the report ----------\n"
              << "close : Close a position by its id\n"
              << "closeall : Close every positions open\n"
              << "printid : Show a position by its id\n"
              << "print : Print every positions still open\n"
              << "speed : Manage time (in seconds) between candles\n"
              << "autotrade : Make auto trade mod enabled or disabled\n" 
              << "pause/resume : Pause and resume the simulation" << std::endl;


    /*Petit delai d'attente le temps de prendre connaissances des commandes*/
    std::this_thread::sleep_for(std::chrono::seconds(8));


    Candle c;
    idShowPos = -1;
    idClosePos = -1;
    int tmp=0;




    /*Simulation en cours*/
    while(market.hasNext() && !stopRequest){
        tmp++;

        while(waitInput && !stopRequest){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        /*Recuperation de la prochaine bougie & Appel de la strategie*/
        c = market.next();
        std::cout << "\n********************* NEXT CANDLE ****************************\n";
        std::cout << "\n\n ----------- [BACKTESTER] NEW CANDLE : "<< c.getDate() << " -----------\n";
        std::cout << "**** CURRENT PRICE IS : " << c.getClose() << " ****\n";


        /*Analyse de la strategie sur la bougie actuelle*/

        std::cout << "\n------- Using strategy : " << strat.getName() << " -------\n";
        auto oreqOpt = strat.onCandle(c, portfolio, market.getAsset());

        /*Execution de l'ordre si recu*/
        if(oreqOpt.has_value()){
            OrderRequest &oreq = oreqOpt.value();

            std::cout << "\n----------- EXECUTION ---------\n";
            if (oreq.side == Side::BUY || oreq.side == Side::SELL){

                std::optional<Order> validOrder = portfolio.prepareOrder(oreq);
                if(validOrder.has_value()){

                    /*Pour confirmer un ordre manuellement*/
                    if(autoTrade != 1){
                        std::cout << "\n[BACKTESTER] Order can be executed, Confirm ? [1/0]\n";
                        waitInput = true;
                        while(waitInput && !stopRequest){
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }

                        if(executeTrade == 1){
                            portfolio.executeOrder(validOrder.value());
                        }
                        else{
                            std::cout << "\n[BACKTESTER] Order will not be executed\n";
                        }
                        
                        executeTrade = -1;
                    }
                    /*Pour execution automatique de l'ordre*/
                    else{
                        portfolio.executeOrder(validOrder.value());
                    }

                }         

            }
        }

        /*Aucune requete d'ordre recu*/
        else{
            std::cout << "\n----- No order has been provided -----\n";   
        }

        /*Mise a jour de toutes les positions*/
        std::cout << "\n----------- POSITIONS UPDATED ---------\n";
        portfolio.refreshAllPos(c);

        std::cout << "\n----------- [BACKTESTER] PORTFOLIO VIEW ---------\n";
        std::cout << portfolio << "\n";        

        /*Execution de commande utilisateur*/

        if(print != -1){
            portfolio.showAllPos();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            print = -1;
        }      

        if(idClosePos != -1){
            portfolio.closingPosition(idClosePos, c);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            idClosePos = -1;
        }

        if(closeAll != -1){
            portfolio.closeAllPos(c);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            closeAll = -1;
        }

        if(idShowPos != -1){
            portfolio.showPos(idShowPos);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            idShowPos = -1;
        }




        /*Attendre un peu avant la prochaine bougie*/
        std::this_thread::sleep_for(std::chrono::seconds(speedLevel));
    }


    /*Fin de la simulation*/
    std::cout << "--------------- [BACKTESTER] ENDING SIMULATION ---------------\n";
    strat.onEnd();
    portfolio.closeAllPos(c);
    if(!stopRequest){
        std::cout << "[BACKTESTER] No more candles in the data provided\n" << 
        "[BACKTESTER] Please, enter 'quit' in the terminal to end simulation" << std::endl;
    }

    stopRequest = true;

    //Joindre le thread au programme principal
    if(listener.joinable())
        listener.join();

    /*Fournir les donnees au PerformenceReport*/
    std::cout << "\n[BACKTESTER] Analyzing performance...\n\n";

    std::cout << "\n----------- [BACKTESTER] PORTFOLIO FINAL VIEW ---------\n";
    std::cout << portfolio << "\n"; 

    PerformanceReport report(portfolio.getPositionsHistory());
    report.generateReport("data/performance.txt", *this);  
    std::cout << "\n[BACKTESTER] Your report is available ! (data/performance.txt)\n";

    std::cout << "[BACKTESTER] Simulation finished.\n" << std::endl;

    return;
}




} //end namespace bt



/*------------------------------------------ FIN Backtester.cpp ----------------------------------------------------*/