#ifndef _utils_hpp_
#define _utils_hpp_
#include <string>


/*OK*/
namespace bt{



enum class Side {
    BUY, 
    SELL,
    NONE
};

enum class StatusO {
    OPEN, 
    EXECUTED, 
    UNCOMPLETED,
    CANCELLED,
    NONE
};

enum class StatusP {
    LONG, 
    SHORT, 
    CLOSED,
    NONE
};


/*Fonctions utilitaires*/
// std::string currentDate();
std::string to_string(Side s);
std::string to_string(StatusO s);
std::string to_string(StatusP s);
int verifCin(double& value);
bool verifCin(int &value, int min, int max);




} //end namespace bt









#endif