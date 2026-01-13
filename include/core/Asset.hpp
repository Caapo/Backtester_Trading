#ifndef _ASSET_HPP_
#define _ASSET_HPP_

#include <string>

namespace bt{

/*---------------------------------------- class Asset ---------------------------------------*/
/*
Un asset est un actif sur lequel on travail durant la simulation. 
Cet actif fournit des donnees de marches (csvPath).
*/

class Asset{

    private:
        std::string category;
        std::string name;
        std::string csvPath;
        int digits;
        double maxLeverage;

    public:
        Asset(const std::string &category_, const std::string &name, const std::string &path, int digits, double maxL);
        Asset() = default;

        const std::string& getCategory() const {return category;}
        const std::string& getName() const {return name;}
        const std::string& getPath() const {return csvPath;}
        int getDigits() const {return digits;}
        double getMaxLeverage() const {return maxLeverage;}
};


} //end namespace bt

#endif