#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "Agr.h"
#include "rapidjson/document.h"
#include "fileio/fileio.h"

using namespace std;
using namespace rapidjson;

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

bool parse_sample(const string& json, vector<UserAction>& actions)
{
    Document doc;

    char c_json[json.length() + 1];
    strcpy(c_json, json.c_str());

    // json validation:
    if(doc.ParseInsitu(c_json).HasParseError()) {
        cout << "doc.ParseInsitu(c_json).HasParseError() - failed" << endl;
        return false;
    }


    // parse objects:
    if(doc.HasMember("actions"))
    {
        if(doc["actions"].IsArray())
        {
            const Value& arr = doc["actions"].GetArray();
            for(SizeType i = 0; i < arr.Size(); i++)
            {

                // fill ua object:
                UserAction ua;
                if(arr[i].HasMember("ts_fact")) {
                    if(arr[i]["ts_fact"].IsInt()) {
                        ua.ts_fact = static_cast<uint32_t>(arr[i]["ts_fact"].GetInt());
                    }
                }

                if(arr[i].HasMember("fact_name")) {
                    if(arr[i]["fact_name"].IsString()) {
                        ua.fact_name = arr[i]["fact_name"].GetString();
                    }
                }

                if(arr[i].HasMember("actor_id")) {
                    if(arr[i]["actor_id"].IsInt()) {
                        ua.actor_id = static_cast<uint32_t>(arr[i]["actor_id"].GetInt());
                    }
                }

                // props:
                if(arr[i].HasMember("props")) {
                    if(arr[i]["props"].IsObject()) {
                        auto props_node = arr[i]["props"].GetObject();
                        for(auto itr = props_node.MemberBegin(); itr != props_node.MemberEnd(); ++itr) {
                            if(itr->value.IsInt())
                                ua.props[itr->name.GetString()] = itr->value.GetInt();
                        }
                    }
                }

                actions.push_back(ua);
            }
        }
        else {
            cout << "doc[\"actions\"].IsArray() - failed" << endl;
            return false;
        }
    }
    else {
        cout << "doc.HasMember(\"actions\") - failed" << endl;
        return false;
    }

    return true;
}

int main(int argc, char** argv) {

    string sample_json;
    if(!fileio::readfile("test_sample.json", sample_json)) {
        cout << "failed to read test_sample.json" << endl;
        return 1;
    }

    vector<UserAction> vua;
    if(!parse_sample(sample_json, vua)) {
        cout << "failed to parse test_sample.json" << endl;
        return 1;
    }

//    auto by_date = sort_date(vua);
//    map<Date, map<string, map<props, int>>> aggregated_info;
//    for(auto p : by_date) {
//        map<string, map<props, int>> by_fact = sort_inside_day(p.second);
//        aggregated_info[p.first] = by_fact;
//    }

//    string json = serialize(aggregated_info);
//    fileio::writefile("serialized.json", json);

    return 0;
}