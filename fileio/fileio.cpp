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