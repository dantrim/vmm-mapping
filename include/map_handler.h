#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

////////////////////////////////////////////////
//
// map_handler
//
// Tool that builds detector and electronics
// mapping given the provided configuration
// XML files
//
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
///////////////////////////////////////////////

//nsw
class DaqConfiguration;
//class Element;

//std/stl
#include <string>
#include <unordered_map>


#warning MAP TYPES COMMENTED OUT
//namespace nsw_map {
//    // { FEB CHANNEL : ELEMENT }
//    typedef std::unordered_map<int, Element> febChanToElementMap;
//    // { FEB ID : { febChanToElementMap } }
//    typedef std::unordered_map<int, febChanToElementMap> febIdToChannelMap;
//}


class MapHandler
{

    public :
        MapHandler();
        virtual ~MapHandler(){};

        bool loadDaqConfiguration(std::string filename);
        DaqConfiguration& config() { return *m_daqConfig; }


    private :
        bool m_dbg;
        bool m_initialized;
        bool m_map_loaded;

        DaqConfiguration* m_daqConfig;

//        nsw_map::febIdToChannelMap m_daq_map;


}; // class



#endif
