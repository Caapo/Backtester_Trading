#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "core/MarketData.hpp"
using std::string;


namespace bt{



MarketData::MarketData(const std::string &pathname, const std::string &asset) : assetName(asset) {
    loadCSV(pathname, asset);
}


int MarketData::loadCSV(const string &path, const string &assetName){

    /*Verifie que le fichier existe*/
    if(!std::filesystem::exists(path)){
        throw std::runtime_error("[MarketData] CSV file does not exist : " + path);
    }

    std::ifstream file(path);
    if(!file.is_open()){
        throw std::runtime_error("[MarketData] Failed to open file : " + path);
    }

    /*Ici, l'ouverture est reussie*/
    this->assetName = assetName; 
    candles.clear();

    /*Skip le header*/
    std::string line;
    std::getline(file, line);
    
    /*Iteration sur les lignes*/
    while(std::getline(file, line)) {
        if(line.empty()) continue;

        /*Stocker dans ss chaque partie (separee par des ',') de la ligne extraite*/
        std::replace(line.begin(), line.end(), '"', ' ');
        std::stringstream ss(line);
        string date, openStr, highStr, lowStr, closeStr;
        
        std::getline(ss, date, ',');
        std::getline(ss, openStr, ',');
        std::getline(ss, highStr, ',');
        std::getline(ss, lowStr, ',');
        std::getline(ss, closeStr, ',');

        try{
            candles.emplace_back(date, std::stod(openStr), std::stod(highStr), std::stod(lowStr), std::stod(closeStr));
        }
        catch(const std::exception& e){
            throw std::runtime_error("[MarketData] parse error: " + std::string(e.what()));
        }
    }
    
    
    file.close();
    
    this->currentIdx = 0;


    return 0;

}




bool MarketData::hasNext() const{ 
    return currentIdx < candles.size(); 
}




const Candle& MarketData::next(){
    if(hasNext() == false) throw std::out_of_range("[DEBUG MarketData::next] No more candles in this simulation.");
    return candles[currentIdx++];
}



void MarketData::reset(){
    candles.clear(); 
    currentIdx = 0;
}


} //end namespace bt