#include <iostream>
#include <map>
#include <vector>
#include "user-action.h"
#include "Date.h"

using namespace std;

struct KeyObject_example {

    KeyObject_example(const Date& date, const std::string& fact_name) {
        this->date = date;
        this->fact_name = fact_name;
    }

    Date date;
    std::string fact_name;
};

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
    vua.emplace_back(1483257600, "fact_name_1"); // Sunday, January 1, 2017 8:00:00 AM
    vua.emplace_back(1483290000, "fact_name_2"); // Sunday, January 1, 2017 5:00:00 PM

    vua.emplace_back(1483376400, "fact_name_1"); // Monday, January 2, 2017 5:00:00 PM

    vua.emplace_back(1483462800, "fact_name_2"); // Tuesday, January 3, 2017 5:00:00 PM
    vua.emplace_back(1483466400, "fact_name_1"); // Tuesday, January 3, 2017 6:00:00 PM



    auto info = sort_user_actions(vua);
    return 0;
}