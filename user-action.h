#ifndef GAMELOGPARSER_USER_ACTION_H
#define GAMELOGPARSER_USER_ACTION_H

#include <stdint-gcc.h>

struct UserAction {

    UserAction(uint32_t ts) {
        ts_fact = ts;
    }

    uint32_t  ts_fact;
};

#endif //GAMELOGPARSER_USER_ACTION_H
