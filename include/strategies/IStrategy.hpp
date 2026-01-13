#ifndef _ISTRATEGY_HPP_
#define _ISTRATEGY_HPP_

#include <optional>
#include "core/Order.hpp"
#include "utils.hpp"
#include <iostream>
#include "core/MarketData.hpp"
#include "core/Candle.hpp"
#include "core/Portfolio.hpp"


namespace bt{


/*------------------------------------------------ class IStrategy ------------------------------------------------*/
/*
IStrategy est une classe abstraite servant d'interface pour les strategies instanciables.
Une strategie renvoie un ordre si elle detecte un signal d'achat ou de vente. 
Si elle n'en detecte pas, aucun ordre n'est envoye.

onStart : Initialisation de la strategie 
onCandle : Coeur de la strategie pour detecter un signal
onEnd : Fin de la strategie
*/

class IStrategy {
    protected:
        static int strategiesCount;
        int id;
        bool trigger{false};
        std::string name;
    
    public:
        
        IStrategy(const std::string &name_);
        virtual ~IStrategy();

        /*Methodes*/
        virtual void onStart() = 0;
        virtual std::optional<OrderRequest> onCandle(const Candle&, const Portfolio&, std::string) = 0;
        virtual void onEnd() = 0;

        /*Getters/Setters*/
        std::string getName(){return name;}

};

} //end namespace bt

#endif