//nsw
#include "map_handler.h"

#include <iostream>
using namespace std;
#include <string>

int main(int argc, char* argv[]) {

    cout << "test mapping" << endl;

    MapHandler* handler = new MapHandler();
    handler->loadDaqConfiguration("/Users/dantrim/workarea/NSW/myreadout/NEWMAPPING/testMapping/DAQ_config_TZ.xml");
    
    return 0;
}
