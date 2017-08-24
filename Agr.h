#ifndef GAMELOGPARSER_AGR_H
#define GAMELOGPARSER_AGR_H

#include <vector>

#include "user-action.h"
#include "Date.h"

typedef std::map<std::string, int> props;

class Agr
{
public:

    // aggregation wrapper method:
    static std::map<Date, std::map<std::string, std::map<props, int>>> aggregate(const std::vector<UserAction>& input);

private:

    // aggregation methods:
    static std::map<Date, std::vector<UserAction>> sort_date(const std::vector<UserAction>& vua);
    static std::map<std::string, std::map<props, int>> sort_inside_day(const std::vector<UserAction>& vua);

};


#endif //GAMELOGPARSER_AGR_H
