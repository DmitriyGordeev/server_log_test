#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <dirent.h>
#include <sys/stat.h>

#include "Agr.h"
#include "rapidjson/document.h"
#include "fileio/fileio.h"
#include "LogLoader.h"

using namespace std;
using namespace rapidjson;


// serialize final aggregated data as json with rapidjson
string serialize(const map<Date, map<string, map<props, int>>>& input) {

    StringBuffer sb;
    PrettyWriter<StringBuffer> pw(sb);

    pw.StartObject();

    for(auto p_date : input)
    {
        time_t ts = p_date.first.timestamp();
        string date_str = ctime(&ts);
        date_str.pop_back();
        pw.Key(date_str.c_str());
        pw.StartArray();

        for(auto p_fact : p_date.second) {
            pw.StartObject();

            pw.Key("fact_name");
            pw.String(p_fact.first.c_str());

            pw.Key("combinations");
            pw.StartArray();
            for(auto p_props : p_fact.second) {
                pw.StartObject();

                pw.Key("props");
                pw.StartObject();
                for(auto p_prop_name : p_props.first) {
                    pw.Key(p_prop_name.first.c_str());
                    pw.Int(p_prop_name.second);
                }
                pw.EndObject();

                pw.Key("count");
                pw.Int(p_props.second);

                pw.EndObject();
            }
            pw.EndArray();

            pw.EndObject();
        }

        pw.EndArray();
    }

    pw.EndObject();
    return sb.GetString();
}


int main(int argc, char** argv)
{
    if(argc != 4) {
        cout << "usage: [logs path] [logs number] [threads number]";
        return 1;
    }

    // get args:
    string logs_dir(argv[1]);
    int logs_num = 0;
    try {
        logs_num = stoi(string(argv[2]));
    }
    catch(exception& e) {
        cout << "second argument must be numeric!" << endl;
        return 1;
    }

    int threads_num = 0;
    try {
        threads_num = stoi(string(argv[3]));
    }
    catch(exception& e) {
        cout << "third argument must be numeric!" << endl;
        return 1;
    }


    vector<UserAction> actions = LogLoader::load("../log-dir/", logs_num, threads_num);

    // aggregate info and output to json
    auto agr_info = Agr::aggregate(actions);
    string json = serialize(agr_info);
    fileio::writefile("agr.json", json);


    return 0;
}