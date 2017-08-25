/* simple read / write file static class */

#ifndef GAMELOGPARSER_FILEIO_H
#define GAMELOGPARSER_FILEIO_H

#include <fstream>
#include <dirent.h>
#include <vector>

class fileio {
public:
    static bool readfile(const std::string& filename, std::string& content);
    static void writefile(const std::string& filename, const std::string& content);
    static bool files_list(const std::string& path, std::vector<std::string>& files);
};


#endif //GAMELOGPARSER_FILEIO_H
