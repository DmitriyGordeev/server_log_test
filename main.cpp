#include <iostream>
#include <map>
#include <vector>
#include "user-action.h"
#include "Date.h"

using namespace std;

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

    auto info = sort_user_actions(vua);


    return 0;
}