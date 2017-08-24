#include "Date.h"

bool operator < (const Date& a, const Date& b) {
    return a.timestamp() < b.timestamp();
}

