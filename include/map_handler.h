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
#include "element.h"

//std/stl
#include <string>
#include <unordered_map>
#include <map>
#include <vector>


#warning MAP TYPES COMMENTED OUT
namespace nsw_map {
    // { FEB CHANNEL : ELEMENT }
    typedef std::unordered_map<int, Element> febChanToElementMap;
    // { FEB ID : { febChanToElementMap } }
    typedef std::unordered_map<int, febChanToElementMap> febIdToChannelMap;

    // container type for building up list of board > [chip] needed
    // for the monitoring to build up its tree
    // { BOARD NAME : vector<VMMNAME> }
    typedef std::map<std::string, std::vector<std::string> > boardContentMap;

}


class MapHandler
{

    public :
        MapHandler();
        virtual ~MapHandler(){};

        bool loadDaqConfiguration(std::string filename);
        DaqConfiguration& config() { return *m_daqConfig; }

        bool buildMapping();

        std::vector<nsw_map::boardContentMap>& boardContents() { return m_board_contents; }


    private :
        bool m_dbg;
        bool m_initialized;
        bool m_map_loaded;

        DaqConfiguration* m_daqConfig;

        nsw_map::febIdToChannelMap m_daq_map;

        std::vector<nsw_map::boardContentMap> m_board_contents;

}; // class



#endif
