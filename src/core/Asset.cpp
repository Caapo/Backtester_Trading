#include "core/Asset.hpp"

namespace bt{


Asset::Asset(const std::string &category_, const std::string& name_, const std::string& path, int digits, double maxL)
: category{category_}, name{name_}, csvPath{path}, digits{digits}, maxLeverage{maxL} {}



}