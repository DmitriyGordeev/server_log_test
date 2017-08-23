#include <iostream>
#include <chrono>
#include <map>
#include <vector>
#include <ctime>
#include "user-action.h"

using namespace std;

struct Date
{
    Date() {}
    Date(time_t ts) {
        convert(ts);
    }

    int year;
    int month;
    int day;

    int hour;
    int min;
    int sec;

    void convert(time_t timestamp) {
        struct tm * date = localtime( & timestamp );

        year = date->tm_year + 1900;
        month = date->tm_mon + 1;
        day = date->tm_mday;

        hour = date->tm_hour - 3;
        min = date->tm_min;
        sec = date->tm_sec;
    }

    time_t timestamp() const {
        struct tm * date = new tm();

        date->tm_year = year - 1900;
        date->tm_mon = month - 1;
        date->tm_mday = day;

        date->tm_hour = hour + 3;
        date->tm_min = min;
        date->tm_sec = sec;

        return mktime(date);
    }

    friend bool operator < (const Date& a, const Date& b);

};

bool operator < (const Date& a, const Date& b) {
    return a.timestamp() < b.timestamp();
}


map<Date, int> sort_user_actions(vector<UserAction> vua) {

    map<Date, int> out;
    for(UserAction ua : vua) {

        Date d(ua.ts_fact);
        d.hour = 0;
        d.min = 0;
        d.sec = 0;

        out[d]++;
    }

    return out;
};

int main() {

    vector<UserAction> vua;
    vua.emplace_back(1483257600); // Sunday, January 1, 2017 8:00:00 AM
    vua.emplace_back(1483290000); // Sunday, January 1, 2017 5:00:00 PM

    vua.emplace_back(1483376400); // Monday, January 2, 2017 5:00:00 PM

    vua.emplace_back(1483462800); // Tuesday, January 3, 2017 5:00:00 PM
    vua.emplace_back(1483466400); // Tuesday, January 3, 2017 6:00:00 PM


    return 0;
}