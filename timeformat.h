#ifndef GAMELOGPARSER_TIMEFORMAT_H
#define GAMELOGPARSER_TIMEFORMAT_H

#include <time.h>
#include <string>

std::string time_format(time_t ts, const char* format) {

    char buffer[80];
    struct tm * timeinfo = localtime(&ts);

    strftime(buffer, 80, format, timeinfo);
    return std::string(buffer);
}

#endif //GAMELOGPARSER_TIMEFORMAT_H
