#ifndef GAMELOGPARSER_FILEIO_H
#define GAMELOGPARSER_FILEIO_H

#include <fstream>

/* simple class for read / write files: */
class fileio {
public:
    static bool readfile(const std::string& filename, std::string& content);
    static void writefile(const std::string& filename, const std::string& content);
};


#endif //GAMELOGPARSER_FILEIO_H
