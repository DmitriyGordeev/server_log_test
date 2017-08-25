#include <iostream>
#include <sys/stat.h>
#include <algorithm>

#include "LogLoader.h"
#include "rapidjson/document.h"
#include "fileio/fileio.h"

using namespace std;
using namespace rapidjson;

vector<UserAction> LogLoader::load(const std::string& logs_dir, size_t num_log_files, size_t num_threads)
{
    if(num_threads < 1)
        num_threads = 1;

    vector<thread> threads(num_threads);
    vector<UserAction> out;
    vector<string> file_names;

    if(fileio::files_list(logs_dir, file_names)) {

        file_names.erase(remove(file_names.begin(), file_names.end(), "."), file_names.end());
        file_names.erase(remove(file_names.begin(), file_names.end(), ".."), file_names.end());

        /* If files count less than specified logs_num it must be handled.
         * By default (num_log_files = 0) take all found log files: */
        size_t log_count = std::min(file_names.size(), num_log_files);
        if(num_log_files == 0) {
            log_count = file_names.size();
        }


        if(log_count < num_threads) {
            num_threads = log_count;
        }


        // create threads:
        for(size_t i = 0; i < file_names.size(); i += num_threads) {

            vector<UserAction> file_actions;
            for (size_t j = 0; j < num_threads; j++) {
                if (i + j < file_names.size()) {
                    threads[j] = thread(single_log, logs_dir + file_names[i + j], ref(file_actions));
                }
            }

            for(size_t j = 0; j < num_threads; j++) {
                if(threads[j].joinable())
                    threads[j].join();
            }

            out.insert(out.end(), file_actions.begin(), file_actions.end());
        }
    }

    return out;
}

// parse input player actions json (see input_example.json)
bool LogLoader::parse_sample(const string& json, vector<UserAction>& actions) {

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


// privates:
// process single log file (for simple threading)
bool LogLoader::single_log(const std::string& path, vector<UserAction>& actions) {

    struct stat buf;
    stat(path.c_str(), &buf);
    if(S_ISDIR(buf.st_mode)) {
        return false;
    }

    string json;
    if(!fileio::readfile(path, json)) {
        return false;
    }

    return parse_sample(json, actions);
}