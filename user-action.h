#ifndef GAMELOGPARSER_USER_ACTION_H
#define GAMELOGPARSER_USER_ACTION_H

#include <map>

struct UserAction {

    UserAction(uint32_t ts_fact, const std::string& fact_name, const std::map<std::string, int>& props) {
        this->ts_fact = ts_fact;
        this->fact_name = fact_name;
        this->props = props;
    }

    uint32_t  ts_fact;
    std::string fact_name;
    std::map<std::string, int> props;
};

#endif //GAMELOGPARSER_USER_ACTION_H
