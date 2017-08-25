/*
 * Class for multithreaded load and parse
 * player actions jsons from given directory
 */

#ifndef GAMELOGPARSER_LOGLOADER_H
#define GAMELOGPARSER_LOGLOADER_H

#include <thread>
#include <vector>

#include "user-action.h"


class LogLoader
{
public:
    static std::vector<UserAction> load(const std::string& log_dir, size_t num_log_files = 0, size_t num_threads = 1);
    static bool parse_sample(const std::string& json, std::vector<UserAction>& actions);

private:
    static bool single_log(const std::string& path, std::vector<UserAction>& actions);
};


#endif //GAMELOGPARSER_LOGLOADER_H
