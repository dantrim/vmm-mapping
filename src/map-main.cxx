//nsw
#include "map_handler.h"

#include <iostream>
using namespace std;
#include <string>

int main(int argc, char* argv[]) {

    cout << "test mapping" << endl;

    MapHandler* handler = new MapHandler();
    handler->loadDaqConfiguration("/Users/dantrim/workarea/NSW/myreadout/NEWMAPPING/testMapping/DAQ_config_TZ.xml");
    handler->buildMapping();


    // boardContents() returns a vector of:
    // vector<nsw_map::boardContentMap> (c.f. map_handler.h)
    // where boardContentMap:
    //  std::map<std::string, std::vector<std::string> >
    //      -> [board name][ { list of VMM names } ]
    for(auto boardidmap : handler->boardContents()) {
        for(auto boardid : boardidmap) {
            cout << "loaded board: " << boardid.first << endl;
            for(auto vmm : boardid.second) {
                cout << "    --> " << vmm << endl;
            }
        }
    }

    
    return 0;
}
