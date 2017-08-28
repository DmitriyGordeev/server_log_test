#include <iostream>
#include <sys/stat.h>
#include <algorithm>
#include <sstream>

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

        if(file_names.empty()) {
            cout << "log folder: \"" << logs_dir << "\" is empty" << endl;
            return out;
        }

        /* If files count less than specified logs_num it must be handled.
         * By default (num_log_files = 0) take all found log files: */
        size_t log_count = std::min(file_names.size(), num_log_files);
        if(num_log_files == 0) {
            log_count = file_names.size();
        }


        if(log_count < num_threads) {
            num_threads = log_count;
        }


        for(size_t i = 0; i < file_names.size(); i += num_threads) {

            vector<vector<UserAction>> file_actions(num_threads);
            for (size_t j = 0; j < num_threads; j++) {
                if (i + j < file_names.size()) {
                    threads[j] = thread(single_log, logs_dir + file_names[i + j], ref(file_actions[j]));
                }
            }

            for(size_t j = 0; j < num_threads; j++) {
                if(threads[j].joinable()) {
                    threads[j].join();
                }
            }

            // collapse vector<vector>> to single vector:
            for(size_t l = 0; l < file_actions.size(); l++) {
                out.insert(out.end(), file_actions[l].begin(), file_actions[l].end());
            }
        }
    }

    return out;
}

// processing single log file:
bool LogLoader::single_log(const std::string& path, vector<UserAction>& actions) {

    string data;
    if(!fileio::readfile(path, data)) {
        return false;
    }

    istringstream is(data);
    string line_json;
    while(getline(is, line_json)) {

        UserAction ua;
        if(ua.parse(line_json)) {
            actions.push_back(ua);
        }
    }

    return true;
}


