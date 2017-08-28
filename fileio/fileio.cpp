#include "fileio.h"
using namespace std;

bool fileio::readfile(const std::string& filename, std::string& content)
{
    ifstream f_input(filename.c_str());
    if(f_input.fail()) {
        return false;
    }

    string line;
    while(getline(f_input, line)) {
        content.append(line + "\n");
    }

    content.pop_back();
    f_input.close();
    return true;
}

void fileio::writefile(const std::string& filename, const std::string& content)
{
    ofstream fout(filename.c_str());
    fout << content;
    fout.close();
}

bool fileio::files_list(const string& path, vector<string>& files) {

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            string name(ent->d_name);
            files.push_back(name);
        }
        closedir (dir);
    } else {
        perror(path.c_str());
        return false;
    }

    return true;
}