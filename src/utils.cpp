#include "utils.hpp"
#include <chrono>
#include <ctime>
#include <iostream>


namespace bt{



/*------------------------ Fonctions de conversion enum => string --------------------------*/
std::string to_string(Side s){
    switch(s){
        case Side::BUY:  return "BUY";
        case Side::SELL: return "SELL";
        case Side::NONE: return "NONE";
    }
    return "UNKNOWN";
}

std::string to_string(StatusO s){
    switch(s){
        case StatusO::OPEN:         return "OPEN";
        case StatusO::EXECUTED:     return "EXECUTED";
        case StatusO::UNCOMPLETED:  return "UNCOMPLETED";
        case StatusO::CANCELLED:    return "CANCELLED";
        case StatusO::NONE:         return "NONE";
    }
    return "UNKNOWN";
}

std::string to_string(StatusP s){
    switch(s){
        case StatusP::LONG:     return "LONG";
        case StatusP::SHORT:    return "SHORT";
        case StatusP::CLOSED:    return "CLOSED";
        case StatusP::NONE:     return "NONE";
    }
    return "UNKNOWN";
}



/*Verifie la conformite du l'entree utilisateur (usage dans le main)*/
int verifCin(double &value){
    
    while(std::cin.fail()){
        std::cout << "Une valeur decimale est attendu, reessayez\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cin >> value;
    }
    return 0;
}



bool verifCin(int &value, int min, int max){
    std::cin >> value;

    if(std::cin.fail() || value < min || value > max){
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        return false;
    }

    std::cin.ignore(1000, '\n');
    return true;
}



} //end namespace bt