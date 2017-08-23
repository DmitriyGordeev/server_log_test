#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "user-action.h"
#include "Date.h"

using namespace std;

typedef map<string, int> props;

/* count duplicates
 * transform multiset into map: key = element, value = repeat count
 */
template <typename T>
map<T, int> unique_counts(multiset<T> in) {
    map<T, int> out;
    for(auto i = in.begin(); i != in.end(); i = in.upper_bound(*i)) {
        out[*i] = in.count(*i);
    }
    return out;
};


map<Date, vector<UserAction>> sort_date(const vector<UserAction>& vua) {

    map<Date, vector<UserAction>> out;
    for(UserAction ua : vua) {

        Date d(ua.ts_fact);
        d.hour = 0; d.min = 0; d.sec = 0;

        out[d].push_back(ua);
    }

    return out;
};


map<string, map<props, int>> sort_inside_day(const vector<UserAction>& vua) {

    map<string, map<props, int>> out;

    // 1. collect by fact_name
    map<string, vector<props>> intermed;
    for(UserAction ua : vua) {
        intermed[ua.fact_name].push_back(ua.props);
    }

    // 2. counting each prop combination for such fact_name
    for(auto p : intermed) {
        map<props, int> duplicate_counts;
        for(auto i : p.second) {
            duplicate_counts[i]++;
        }
        out[p.first] = duplicate_counts;
    }

    return out;
};


int main() {

    map<string, int> m1;
    m1["prop_name1"] = 1;
    m1["prop_name2"] = 2;
    m1["prop_name3"] = 3;

    map<string, int> m2;
    m2["prop_name1"] = 11;
    m2["prop_name2"] = 22;
    m2["prop_name3"] = 33;

    map<string, int> m3;
    m3["prop_name1"] = 111;
    m3["prop_name2"] = 222;
    m3["prop_name3"] = 333;


    vector<UserAction> vua;
    vua.emplace_back(1483257600, "fact_name_1", m1); // Sunday, January 1, 2017 8:00:00 AM
    vua.emplace_back(1483290000, "fact_name_2", m2); // Sunday, January 1, 2017 5:00:00 PM
    vua.emplace_back(1483296840, "fact_name_1", m1); // Sunday, January 1, 2017 6:54:00 PM

    vua.emplace_back(1483376400, "fact_name_1", m3); // Monday, January 2, 2017 5:00:00 PM

    vua.emplace_back(1483462800, "fact_name_2", m3); // Tuesday, January 3, 2017 5:00:00 PM
    vua.emplace_back(1483466400, "fact_name_1", m3); // Tuesday, January 3, 2017 6:00:00 PM


    auto by_date = sort_date(vua);
    map<Date, map<string, map<props, int>>> aggregated_info;
    for(auto p : by_date) {
        map<string, map<props, int>> by_fact = sort_inside_day(p.second);
        aggregated_info[p.first] = by_fact;
    }


    return 0;
}