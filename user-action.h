#ifndef GAMELOGPARSER_USER_ACTION_H
#define GAMELOGPARSER_USER_ACTION_H

struct UserAction {

    UserAction(uint32_t ts_fact, const std::string& fact_name) {
        this->ts_fact = ts_fact;
        this->fact_name = fact_name;
    }

    uint32_t  ts_fact;
    std::string fact_name;
};

#endif //GAMELOGPARSER_USER_ACTION_H
