#include "Agr.h"

using namespace std;

std::map<Date, std::map<std::string, std::map<props, int>>> Agr::aggregate(const std::vector<UserAction>& input)
{
    auto date_mapping = sort_date(input);
    map<Date, map<string, map<props, int>>> agr_info;

    for(auto p : date_mapping) {
        map<string, map<props, int>> fact_mapping = sort_inside_day(p.second);
        agr_info[p.first] = fact_mapping;
    }
    return agr_info;
};

map<Date, vector<UserAction>> Agr::sort_date(const vector<UserAction>& vua) {

    map<Date, vector<UserAction>> out;
    for(UserAction ua : vua) {

        Date d(ua.ts_fact);
        d.hour = 0; d.min = 0; d.sec = 0;

        out[d].push_back(ua);
    }

    return out;
};

map<string, map<props, int>> Agr::sort_inside_day(const vector<UserAction>& vua) {

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