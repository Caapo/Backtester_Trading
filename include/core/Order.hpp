#ifndef _Order_hpp_
#define _Order_hpp_
#include "utils.hpp"
#include <string>


namespace bt{

/*---------------------------------------- struct OrderRequest -------------------------------------------*/
/*
Un OrderRequest est cree par une Strategie si elle detecte un signal de vente ou d'achat.
Il fournit les informations necessaire pour la creation d'un ordre par le Portfolio.
*/

struct OrderRequest {
    public :
        int id{-1};
        std::string date{""};
        std::string asset{""};
        Side side{Side::NONE};
        double entryPrice{-1.0};
        double RRR{0.0};
        double quantity{-1.0};
        double stopLoss{-1.0};
        double takeProfit{-1.0};

        /*Membres speciaux*/
        OrderRequest() = default;
        OrderRequest(int id_, const std::string &date_, const std::string &asset_, Side side_, double entryPrice_, 
        double RRR_, double quantity_, double stopLoss_, double takeProfit_);

};


/*------------------------------------------ class Order --------------------------------------------*/
/*
Un Ordre est cree par le Portfolio suite a une requete d'ordre provenant d'une Strategie.
Un Ordre est une demande d'execution au marche.
S'il est execute, une nouvelle Position (exposition reelle sur le marche) est cree.
Dans le cas contraire, aucune Position n'est cree. 
Un ordre non executee n'a aucune influence sur le capital. Seule une Position impacte le Portfolio.
*/

class Order{

friend std::ostream& operator<<(std::ostream &os, const Order &p);
    

private:
    int id;
    std::string date{""};
    std::string asset{""};
    Side side{Side::NONE};
    double entryPrice{0.0};
    double quantity{0.0};
    double stopLoss{0.0};
    double takeProfit{0.0};
    StatusO status{StatusO::NONE};

    static int orderNb;
    

public:

    Order();
    Order(const std::string &date_, const std::string &asset_, Side side_, double entryPrice_, double quantity_,  StatusO status_);

    // Order(const Order&) = default;
    // Order(Order&&) = default;

    // Order& operator=(const Order&) = default;
    // Order& operator=(Order&&) = default;
    
    // ~Order() = default;

    /*Getters/Setters*/
    int getID() const { return id; }
    const std::string& getAsset() const { return asset; }
    double getTakeProfit() const { return takeProfit; }
    double getStopLoss() const { return stopLoss; }
    const std::string& getDate() const { return date; }
    Side getSide() const { return side; }
    StatusO getStatus() const { return status; }
    void setStatus(const StatusO &status_);
    double getEntryPrice() const { return entryPrice; }
    double getQuantity() const { return quantity; }
    bool setTakeProfit(double takeProfit_);
    bool setStopLoss(double stopLoss_); 


};


} //end namespace bt


#endif