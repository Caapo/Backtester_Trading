#ifndef _CANDLE_HPP_
#define _CANDLE_HPP_

#include <string>


namespace bt{


/*---------------------------------------- class Candle ---------------------------------------*/
/*
Une bougie represente l'evolution du prix sur une unite de temps donnee.
Exemple : Sur un graphique Daily, une bougie represente un jour et renseigne le prix d'ouverture, 
le prix le plus haut, le plus bas, et le prix de fermeture au cours de ce jour.
*/

class Candle{

    friend std::ostream& operator << (std::ostream &os, const Candle &c);

    private : 
        std::string date;
        double open, high, low, close;


    public :        
        Candle(const std::string &d, double o, double h, double l, double c);
        Candle() = default;

        // Candle(const Candle&) = default;
        // Candle(Candle&&) = default;

        // Candle& operator=(const Candle&) = default;
        // Candle& operator=(Candle&&) = default;

        // ~Candle() = default;


        /*Getters/Setters*/
        const std::string& getDate() const { return date; };
        double getOpen() const { return open; }
        double getHigh() const { return high; }
        double getLow() const { return low; }
        double getClose() const { return close; }

    


};


} //end namespace bt

#endif