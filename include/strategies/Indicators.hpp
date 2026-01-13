#ifndef _INDICATORS_HPP_
#define _INDICATORS_HPP_

#include <numeric>
#include <iterator>
#include <cmath>
#include <vector>

/*Le fichier Indicators permet aux strategies d'utiliser des indacteurs techniques
qui ne sont pas forcement issus du trading mais qui effectue simplement un traitement sur des donnees 
recues, ces indicateurs sont generiques et peuvent donc servir en dehors de la simulation*/

namespace bt{


/*Moyenne mobile simple sur une periode*/
template<typename T>
double SMAIDC(const std::vector<T>& data, size_t period) {
    if (data.size() < period) return 0.0;
    auto startIt = data.end() - period;
    double sum = std::accumulate(startIt, data.end(), 0.0);
    return sum / static_cast<double>(period);
}


/*Ecart-type sur une periode*/
template<typename T>
double StdDevIDC(const std::vector<T>& data, size_t period) {
    if(data.size() < period) return 0.0;
    double mean = SMAIDC(data, period);
    double variance = 0.0;
    for(auto it = data.end() - period; it != data.end(); ++it) {
        variance += (*it - mean) * (*it - mean);
    }
    variance /= period;
    return std::sqrt(variance);
}



/*Momentum (Close - Close[n])*/
template<typename T>
double MomentumIDC(const std::vector<T>& data, size_t period) {
    if(data.size() <= period) return 0.0;
    return data.back() - data[data.size() - period - 1];
}


} //end namespace bt


#endif