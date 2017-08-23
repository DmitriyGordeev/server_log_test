#include <iostream>
#include <string>
#include <map>
#include "../rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

struct UserAction
{
    uint32_t ts_fact;
    string fact_name;
    uint32_t actor_id;
    map<string, int> props;

    // simple serialization
    void json(PrettyWriter<StringBuffer>& pw) {

        pw.StartObject();

        pw.Key("ts_fact");
        pw.Int(ts_fact);

        pw.Key("fact_name");
        pw.String(fact_name.c_str());

        pw.Key("actor_id");
        pw.Int(actor_id);

        pw.Key("props");
        pw.StartArray();
        for(pair<string, int> p : props) {
            pw.Key(p.first.c_str());
            pw.Int(p.second);
        }
        pw.EndArray();

        pw.EndObject();

    }
};

int main(int argc, char** argv) {

    int log_file_number = 10;

    UserAction ua;
    ua.ts_fact = 1489494303;
    ua.fact_name = "this is fact!";
    ua.actor_id = 31324232;

    ua.props["one"] = 1;
    ua.props["two"] = 2;
    ua.props["three"] = 3;

    StringBuffer sb;
    PrettyWriter<StringBuffer> pw(sb);

    ua.json(pw);

    cout << sb.GetString() << endl;

    return 0;
}

