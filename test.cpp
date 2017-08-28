#include <iostream>

#include "split.h"
#include "user-action.h"
#include "fileio/fileio.h"

using namespace std;
using namespace rapidjson;

vector<UserAction> load_logs(const string& data)
{
    vector<UserAction> output;

    istringstream is(data);
    string line_json;
    while(getline(is, line_json)) {

        UserAction ua;
        if(ua.parse(line_json)) {
            output.push_back(ua);
        }
    }

    return output;
}


int main() {

    string data;
    if(!fileio::readfile("../test_input.log", data)) {
        cout << "failed to read file" << endl;
        return 1;
    }

    auto vua = load_logs(data);

    return 0;
}
