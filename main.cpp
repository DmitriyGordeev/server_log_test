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


//// get list of files on specified path
//bool list_files(const string& path, vector<string>& files) {
//
//    DIR *dir;
//    struct dirent *ent;
//    if ((dir = opendir (path.c_str())) != NULL) {
//        while ((ent = readdir (dir)) != NULL) {
//            string name(ent->d_name);
//            files.push_back(name);
//        }
//        closedir (dir);
//    } else {
//        perror ("");
//        return false;
//    }
//
//    return true;
//}
//
//
//// look into dir, load log and parse to single vector<UserAction>
//vector<UserAction> load_logs(const string& path, size_t logs_num = 0)
//{
//    vector<UserAction> out;
//    vector<string> file_names;
//    if(list_files(path, file_names)) {
//
//        file_names.erase(remove(file_names.begin(), file_names.end(), "."), file_names.end());
//        file_names.erase(remove(file_names.begin(), file_names.end(), ".."), file_names.end());
//
//
//        /* If files count less than specified logs_num it must be handled.
//         * By default (log_num = 0) take all found log files: */
//        size_t log_count = std::min(file_names.size(), logs_num);
//        if(logs_num == 0) {
//            log_count = file_names.size();
//        }
//
//
//        for(int i = 0; i < log_count; i++)
//        {
//            /* if suddenly trying to read dir as file, it will be skipped: */
//            struct stat buf;
//            stat((path + file_names[i]).c_str(), &buf);
//            if(S_ISDIR(buf.st_mode)) {
//                continue;
//            }
//
//            string json;
//            if(!fileio::readfile(path + file_names[i], json)) {
//                continue;
//            }
//
//            vector<UserAction> file_actions;
//            if(!parse_sample(json, file_actions)) {
//                continue;
//            }
//
//            out.insert(out.end(), file_actions.begin(), file_actions.end());
//        }
//    }
//
//    return out;
//}


int main(int argc, char** argv)
{
    vector<UserAction> vua = LogLoader::load("../log-dir/", 4, 1);


//    vector<UserAction> actions = load_logs("../log-dir/");
//
//    // aggregate info and output to json
//    auto agr_info = Agr::aggregate(actions);
//    string json = serialize(agr_info);
//    fileio::writefile("agr.json", json);


    return 0;
}