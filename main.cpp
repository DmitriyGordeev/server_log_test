#include <iostream>
#include <map>
#include <vector>
#include "user-action.h"
#include "Date.h"

using namespace std;

typedef map<Date, map<string, map<vector<int>, int>>> ActionsDataBase;

ActionsDataBase sort_actions(vector<UserAction> vua)
{
    ActionsDataBase info;
    for(UserAction ua : vua) {
        Date d(ua.ts_fact);
        d.sec = 0; d.min = 0; d.hour = 0;

        info[d][ua.fact_name]++;
    }

    return info;
};


int main() {

    vector<UserAction> vua;
    vua.emplace_back(1483257600, "fact_name_1"); // Sunday, January 1, 2017 8:00:00 AM
    vua.emplace_back(1483290000, "fact_name_2"); // Sunday, January 1, 2017 5:00:00 PM
    vua.emplace_back(1483296840, "fact_name_2"); // Sunday, January 1, 2017 6:54:00 PM

    vua.emplace_back(1483376400, "fact_name_1"); // Monday, January 2, 2017 5:00:00 PM

    vua.emplace_back(1483462800, "fact_name_2"); // Tuesday, January 3, 2017 5:00:00 PM
    vua.emplace_back(1483466400, "fact_name_1"); // Tuesday, January 3, 2017 6:00:00 PM

    auto info = sort_actions(vua);
    return 0;
}