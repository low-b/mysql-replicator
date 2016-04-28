#include "chunk_list.h"
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <stdint.h>

using namespace std;
using namespace binlog_listener;
int main() {
    CHUNKLIST_5(uint8_t, uint32_t, uint64_t, string, uint32_t) cl;
    stringstream ss;
    ss << (uint8_t) 1;
    ss << (uint32_t) 10000000;
    ss << (uint64_t) 1000;
    ss << string("adfasdfasd");
    ss << (uint32_t) 78234723;
    ss >> cl;
    cout << cl.t << endl << cl.u.t << endl;
    return 0;
}
