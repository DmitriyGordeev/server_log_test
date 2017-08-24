#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "user-action.h"
#include "fileio/fileio.h"

using namespace std;
using namespace rapidjson;

inline uint32_t random_int(uint32_t min, uint32_t max) {
    return rand() % (max - min + 1) + min;
}

struct Randomizer {

    Randomizer(uint32_t ts_min, uint32_t ts_max,
               const vector<string>& fact_name_var,
               const vector<map<string, int>>& prop_var)
    {
        this->ts_min = ts_min;
        this->ts_max = ts_max;

        fact_name_variants = fact_name_var;
        prop_variants = prop_var;
    }

    UserAction random_action()
    {
        uint32_t ts_fact = random_int(ts_min, ts_max);
        string fact_name = fact_name_variants[random_int(0, fact_name_variants.size() - 1)];
        map<string, int> props = prop_variants[random_int(0, prop_variants.size() - 1)];
        uint32_t actor_id = 122423;

        return UserAction(ts_fact, fact_name, actor_id, props);
    }

    uint32_t ts_min;
    uint32_t ts_max;

    vector<string> fact_name_variants;
    vector<map<string, int>> prop_variants;

};

string serialize_action_array(const vector<UserAction>& vua) {

    StringBuffer sb;
    PrettyWriter<StringBuffer> pw(sb);

    pw.StartObject();
    pw.Key("actions");
    pw.StartArray();

    for(auto ua : vua) {
        ua.json(pw);
    }

    pw.EndArray();
    pw.EndObject();
    return sb.GetString();
}



int main(int argc, char** argv) {

    srand(time(NULL));

    int action_NUM = 10;

    uint32_t ts_min = 1485337478; // Wednesday, January 25, 2017 9:44:38 AM
    uint32_t ts_max = 1485769478; // Monday,    January 30, 2017 9:44:38 AM

    vector<string> fact_name_variants { "action_1", "action_2", "action_3" };

    map<string, int> props_1;
    props_1["p1"] = 1;
    props_1["p2"] = 2;
    props_1["p3"] = 3;

    map<string, int> props_2;
    props_2["p1"] = 11;
    props_2["p2"] = 22;
    props_2["p3"] = 33;

    map<string, int> props_3;
    props_3["p1"] = 111;
    props_3["p2"] = 222;
    props_3["p3"] = 333;

    vector<map<string, int>> prop_variants { props_1, props_2, props_3 };

    Randomizer r(ts_min, ts_max, fact_name_variants, prop_variants);

    vector<UserAction> test_sample;
    for(int i = 0; i < action_NUM; i++) {
        test_sample.push_back(r.random_action());
    }

    // serialization and write to file:
    string sample_json = serialize_action_array(test_sample);
    fileio::writefile("test_sample.json", sample_json);

    return 0;
}

