#ifndef GAMELOGPARSER_SPLIT_H
#define GAMELOGPARSER_SPLIT_H

#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &str, char delim)
{
    std::istringstream iss(str);
    std::string tmp;
    std::vector<std::string> res;
    while(getline(iss, tmp, delim)) res.push_back(tmp);
    return res;
}



#endif //GAMELOGPARSER_SPLIT_H
