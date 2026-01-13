#include <iostream>
#include "core/Candle.hpp"

/*OK*/

namespace bt{


Candle::Candle(const std::string &d, double o, double h, double l, double c) : date(d), open(o), high(h), low(l), close(c) {}

std::ostream& operator << (std::ostream &os, const Candle &c){
    os << "candle[D: " << c.date 
    << " O: " << c.open 
    << " H: " << c.high 
    << " L: " << c.low 
    << " C: " << c.close 
    << "]" << "\n";

    return os;
}

}