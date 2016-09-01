//nsw
#include "map_handler.h"
#include "daq_config.h"

//std/stl
#include <iostream>
using namespace std;


//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
//  MapHandler
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
MapHandler::MapHandler() :
    m_dbg(false),
    m_initialized(false),
    m_map_loaded(false),
    m_daqConfig(0)
{
}
// ------------------------------------------------------------------------ //
bool MapHandler::loadDaqConfiguration(string filename)
{
    if(m_daqConfig) delete m_daqConfig;

    m_daqConfig = new DaqConfiguration();

    bool ok = m_daqConfig->loadDaqXml(filename);
    if(ok) {
        cout << "MapHandler::loadDaqConfiguration    DAQ XML loaded: " << filename << endl;
    }
    else {
        cout << "MapHandler::loadDaqConfiguration    Unable to load DAQ XML: " << filename << endl;
        return false;
    }
    ok = m_daqConfig->loadDetectorSetup();
    if(ok) {
        cout << "MapHandler::loadDaqConfiguration    Detector setup loaded successfully" << endl;
    }
    else {
        cout << "MapHandler::loadDaqConfiguration    Unable to load detector setup" << endl;
        return false;
    }

    m_initialized = ok;

    if(!ok) delete m_daqConfig;
    return ok;

}
