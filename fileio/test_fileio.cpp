#include "fileio.h"
#include <iostream>

int main(int argc, char** argv) {

    std::string filename = "test-file.txt";
    std::string test_str = "Some test content";
    fileio::writefile(filename, test_str);

    std::string read_content;
    fileio::readfile(filename, read_content);

    if(read_content == test_str) {
        std::cout << "OK, test passed" << std::endl;
    }
    else {
        std::cout << "TEST FAILED" << std::endl;
    }

    return 0;
}

