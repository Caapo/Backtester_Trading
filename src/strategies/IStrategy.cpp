#include "strategies/IStrategy.hpp"


namespace bt{

IStrategy::IStrategy(const std::string &name_) : id{strategiesCount++}, name{name_} {}
IStrategy::~IStrategy(){
    // std::cout << "IStrategy detruit" << std::endl;
}


} //end namespace bt