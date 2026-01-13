#include <iostream>
#include "core/Portfolio.hpp"
#include <cmath>   
#include <limits>   




namespace bt{



Portfolio::Portfolio(double capital_, double riskPourcentage_, double RRR_, double leverage_) 
: capital{capital_}, margin{capital_}, riskPourcentage{riskPourcentage_}, RRR{RRR_}, 
riskAmount{capital*(riskPourcentage/100)}, leverage{leverage_} {}


/*---------------------------------- prepareOrder -------------------------------*/
/*
Verifie la coherence des ordres (SL et TP) et si l'ordre peut etre execute (fonds suffisant pour ouvrir une position).
Un ordre n'est pas forcement retourner.
*/

std::optional<Order> Portfolio::prepareOrder(OrderRequest &oreq){
    if(oreq.side != Side::BUY && oreq.side != Side::SELL){
        std::cout << "No order request provided by any strategy\n"; 
        return std::nullopt;
    }
   

    double diff = std::fabs(oreq.entryPrice - oreq.stopLoss);
    
    /*Creation de l'ordre*/
    double quantity{1.0};
    quantity = this->riskAmount / diff;
    oreq.quantity = quantity;
    
    Order order(oreq.date, oreq.asset, oreq.side, oreq.entryPrice, oreq.quantity, StatusO::NONE);
    double PosValue = oreq.quantity * oreq.entryPrice;
    double marginRequired = PosValue/leverage;

    std::cout << "\n[ENGINE] Verifying order...\n";    

    /*Sinon, Margin suffisante ?*/    
    if(this->margin < marginRequired){
        std::cout << "Margin needed : " << marginRequired << "\n";
        std::cout << "Margin available : " << margin << "\n";
        std::cout << "[ENGINE] Not enough margin to execute this order. Order cancelled.\n";
        order.setStatus(StatusO::CANCELLED);
        return std::nullopt;
    }
    
    /*Ici : Margin suffisante*/
    order.setStatus(StatusO::OPEN);


    /*Ordre coherent ?*/
    if(order.setTakeProfit(oreq.takeProfit) == false){
        std::cout << "[ENGINE] Invalid Take Profit. Order cancelled.\n";
        order.setStatus(StatusO::CANCELLED);
        return std::nullopt;
    }
    if(order.setStopLoss(oreq.stopLoss) == false){
        std::cout << "[ENGINE] Invalid Stop Loss. Order cancelled.\n";
        order.setStatus(StatusO::CANCELLED);
        return std::nullopt;
    }

    //Ordre coherent
    std::cout << order;
    return order;    
}


/*---------------------------------- excuteOrder -------------------------------*/
/*
Transforme un ordre en une position reelle (exposition reelle sur le marche). 
Met a jour le portfolio car l'ouverture d'une position entraine une consommation de margin.
*/

int Portfolio::executeOrder(Order &order){


    double PosValue = order.getQuantity() * order.getEntryPrice();
    double marginRequired = PosValue/leverage;

    this->updatePortfolio(0.0, 0.0, marginRequired); //Consomme de la margin
    StatusO oStatus = this->updatePositionsVect(order);
    
    if(oStatus == StatusO::EXECUTED){
        std::cout << "\n[ENGINE] Your order has been sucessfully CREATED and EXECUTED\n";
    }
    else{
        std::cout << "\n[ENGINE] Something went wrong during execution... Order " << to_string(oStatus) << "\n";
    }
    order.setStatus(oStatus);


    
    /*Affichage de la consequence de l'ordre*/
    if(oStatus != order.getStatus()) 
        order.setStatus(oStatus);
    
    ordersHistory.push_back(order);
    
    /*Affichage de la position liee a l'ordre*/
    std::cout << "\n------ POSITION CREATED -----\n";
    for (auto it = positions.begin(); it != positions.end();) {
        if(it->getID() == order.getID() && it->getAsset() == order.getAsset()){
            std::cout << *it;
        }
        it++;
    }
    

    return 0;

}


/*---------------------------------- updatePositionsVect -------------------------------*/
/*
- Met a jour le vecteur de position du portfolio en creant une position pour l'ordre recu
- 1 ordre execute = 1 position
*/


StatusO Portfolio::updatePositionsVect(const Order &o){
    
    if(o.getSide() != Side::BUY && o.getSide() != Side::SELL){
        throw std::invalid_argument("Invalid side provided\n");
        return StatusO::CANCELLED;
    }


    /*Creation de la positon et mise a jour du vecteur de positions*/
    StatusP statusP = (o.getSide() == Side::BUY) ? StatusP::LONG : StatusP::SHORT; 
    Position newPos = Position(o.getID(), o.getAsset(), o.getQuantity(), o.getEntryPrice(), o.getDate(), 
                    o.getStopLoss(), o.getTakeProfit(), statusP);
    this->positions.push_back(newPos);

    return StatusO::EXECUTED;

}




/*---------------------------------- updatePortfolio -------------------------------*/
/*
- Recoit une plus-value, une margin eventuellement liberee (si position cloturee), 
et une taille de position (si consommation de margin avec une nouvelle position)
- met a jour les attributs du portfolio(capital, marginS)
- Verifie si les fonds sont suffisant pour continuer la simulation
*/

int Portfolio::updatePortfolio(double pnlRealized_, double marginFreed_, double marginRequired_){
    
    if(marginRequired_ < 0.0){
        std::cerr << "[DEBUG Portfolio::updatePortfolio] A position value cannot be lower than 0.0" << std::endl;
        return 1;
    }

    this->capital += pnlRealized_;
    this->marginUsed -= marginFreed_; //libere de la marge
    this->marginUsed += marginRequired_; //consomme de la marge

    if (marginUsed > capital){
        std::cerr << "[DEBUG Portfolio::updatePortfolio] Margin used exceeds capital" << std::endl;
        //marginUsed = capital;
        return 1;
    }

    this->margin = capital - marginUsed; //marge disponible

    /*Plus de fonds ni de position en cours pouvant en liberer => Compte liquide*/
    if((capital<=0.0) && marginUsed <=0.0){
        capital = 0.0;
        if(positions.size() == 0){
            std::cout << "Your account has been liquidated, ending simulation..." << std::endl;
            return 1;
        }
    }

    return 0;

}


/*---------------------------------- refreshAllPos -------------------------------*/
/*
- Recoit une bougie
- Pour chaque position, appelle sa mise a jour a un nouveau prix
- Appelle updatePortfolio pour le mettre a jour et met a jour le status d'une position (... => CLOSED)
- Supprime la position du vecteur si elle est cloturee apres l'avoir mise l'historique
*/

CloseResult Portfolio::refreshAllPos(const Candle &c){

    CloseResult result;
    if(this->positions.size() > 0){
        for(auto it = positions.begin(); it != positions.end();){
            it->refreshPnlUnrealized(c);
            result = it->update(c, leverage);
            // double realMarginFreed = result.marginFreed / this->leverage;
            updatePortfolio(result.pnlRealized, result.marginFreed, 0.0);//actualise pnl et margin du portfolio
            if(it->getStatus() == StatusP::CLOSED){
                this->positionsHistory.push_back(*it);
                it = this->positions.erase(it);
            }
            else{
                it++;
            }
            
        }
    }
    else{
        std::cout <<  "[DEBUG Portfolio::refreshAllPos] Nothing to refresh\n";
    }

    return result;
}



int Portfolio::winningTrades() const{
    int cpt=0;
    for(const auto &p : positionsHistory){
        if(p.getPnlRealized() >= 0.0){
            cpt++;
        }
    }
    return cpt;
}


int Portfolio::losingTrades() const {
    int cpt=0;
    for(const auto &p : positionsHistory){
        if(p.getPnlRealized() < 0.0){
            cpt++;
        }
    }
    return cpt;
}


void Portfolio::closingPosition(int id, const Candle &c){
    std::cout << "\n------- [PORTFOLIO=>COMMANDS] COMMAND RESULTS -------\n";
    CloseResult result;
    int flag = 0;
    if(this->positions.size() > 0){
        for(auto it = positions.begin(); it != positions.end();){
            if(it->getID() == id){
                result = it->close(c.getDate(), c.getClose(), it->getQuantity(), leverage);
                flag = 1;
                updatePortfolio(result.pnlRealized, result.marginFreed, 0.0);//actualise pnl et margin du portfolio
                if(it->getStatus() == StatusP::CLOSED){
                    this->positionsHistory.push_back(*it);
                    it = this->positions.erase(it);
                }
                std::cout << "[PORTFOLIO=>COMMANDS] Position(ID=" << id << ") has been closed\n";
                break;                
            }
            else{
                it++;
            }
        }

        if(flag == 0){
            std::cout << "[PORTFOLIO=>COMMANDS] No position with id : " << id << "\n";
            std::cout << "[PORTFOLIO=>COMMANDS] No position closed\n";
        }
    }

}

/*---------------------------------- closeAllPos -------------------------------*/
/*
- Ferme toutes les positions en cours
*/

void Portfolio::closeAllPos(const Candle &c){
    CloseResult result;
    if(this->positions.size() > 0){
        for(auto it=positions.begin(); it!= positions.end();){
            result = it->close(c.getDate(), c.getClose(), it->getQuantity(), leverage);
            updatePortfolio(result.pnlRealized, result.marginFreed, 0.0);//actualise pnl et margin du portfolio
            if(it->getStatus() == StatusP::CLOSED){
                this->positionsHistory.push_back(*it);
                it = this->positions.erase(it);
            }
        }
        std::cout << "[BACKTESTER] Every current positions were closed\n";
    }
    marginUsed = 0.0;
    margin = capital;
}



void Portfolio::showPos(int id){
    std::cout << "\n------- [PORTFOLIO=>COMMANDS] COMMAND RESULTS -------\n";
    int flag = 0;
    if(this->positions.size() > 0){
        for(auto it = positions.begin(); it != positions.end(); it++){
            if(it->getID() == id){
                std::cout << *it;
                flag = 1;
            }
        }
    }
    if(flag == 0){
        std::cout << "[PORTFOLIO=>COMMANDS] No position with id : " << id << "\n";
        std::cout << "[PORTFOLIO=>COMMANDS] Maybe already closed ?\n";  
    }
}


void Portfolio::showAllPos(){
    std::cout << "\n------- [PORTFOLIO=>COMMANDS] COMMAND RESULTS -------\n";
    if(this->positions.size() > 0){
        for(auto it = positions.begin(); it != positions.end(); it++){
            std::cout << *it;
        }
    }
}


/*Cette surcharge d'operateur :
- Affiche le portfolio
*/
std::ostream& operator<<(std::ostream &os, const Portfolio &p){
    os << "\nPortfolio Informations\n" 
    << "Capital: " << p.capital
    << " | Margin(Avalaible): " << p.margin
    << " | Margin(Used): " << p.marginUsed <<
    "\n" << "Risk Pourcentage: " << p.riskPourcentage
    << " | RRR: " << p.RRR
    << " | Risk Amount: " << p.riskAmount <<
    "\n";
    return os;
}






} //end namespace bt


