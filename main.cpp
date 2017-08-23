#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

struct Date
{
    Date() {}
    Date(time_t ts) {
        convert(ts);
    }

    int year;
    int month;
    int day;

    int hour;
    int min;
    int sec;

    void convert(time_t timestamp) {
        struct tm * date = localtime( & timestamp );

        year = date->tm_year + 1900;
        month = date->tm_mon + 1;
        day = date->tm_mday;

        hour = date->tm_hour - 3;
        min = date->tm_min;
        sec = date->tm_sec;
    }

    time_t timestamp() {
        struct tm * date = new tm();

        date->tm_year = year - 1900;
        date->tm_mon = month - 1;
        date->tm_mday = day;

        date->tm_hour = hour + 3;
        date->tm_min = min;
        date->tm_sec = sec;

        return mktime(date);
    }

};


int main() {

    time_t t = time(0);
    Date d(t);

    return 0;
}