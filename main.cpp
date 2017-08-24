#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

#include "Agr.h"
#include "rapidjson/document.h"
#include "fileio/fileio.h"

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


// parse input player actions json (see input_example.json)
bool parse_sample(const string& json, vector<UserAction>& actions) {

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


// get list of files on specified path
bool list_files(const string& path, vector<string>& files) {

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            string name(ent->d_name);
            files.push_back(name);
        }
        closedir (dir);
    } else {
        perror ("");
        return false;
    }

    return true;
}


// look into dir, load log and parse to single vector<UserAction>
vector<UserAction> load_logs(const string& path, size_t logs_num = 0)
{
    vector<UserAction> out;
    vector<string> file_names;
    if(list_files(path, file_names)) {

        file_names.erase(remove(file_names.begin(), file_names.end(), "."), file_names.end());
        file_names.erase(remove(file_names.begin(), file_names.end(), ".."), file_names.end());


        /* If files count less than specified logs_num it must be handled.
         * By default (log_num = 0) take all found log files: */
        size_t log_count = std::min(file_names.size(), logs_num);
        if(logs_num == 0) {
            log_count = file_names.size();
        }


        for(int i = 0; i < log_count; i++)
        {
            /* if suddenly trying to read dir as file, it will be skipped: */
            struct stat buf;
            stat((path + file_names[i]).c_str(), &buf);
            if(S_ISDIR(buf.st_mode)) {
                continue;
            }

            string json;
            if(!fileio::readfile(path + file_names[i], json)) {
                continue;
            }

            vector<UserAction> file_actions;
            if(!parse_sample(json, file_actions)) {
                continue;
            }

            out.insert(out.end(), file_actions.begin(), file_actions.end());
        }
    }

    return out;
}


int main(int argc, char** argv) {

    vector<UserAction> actions = load_logs("../log-dir/");

    // aggregate info and output to json
    auto agr_info = Agr::aggregate(actions);
    string json = serialize(agr_info);
    fileio::writefile("agr.json", json);


    return 0;
}