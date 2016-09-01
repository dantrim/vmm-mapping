//nsw
#include "daq_config.h"
#include "det_config.h"

//std/stl
#include <iostream>
#include <fstream>
using namespace std;

//boost
#include <boost/filesystem.hpp>


//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
//  DaqConfiguration
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
DaqConfiguration::DaqConfiguration() :
    m_dbg(false),
    m_map_dir(""),
    m_daq_config_file(""),
    m_detConfig(0)
{
}
// ------------------------------------------------------------------------ //
bool DaqConfiguration::loadDaqXml(string filename)
{

    string full_filename = filename;
    bool exists = std::ifstream(full_filename).good();

    if(!exists) {
        cout << "DaqConfiguration::loadDaqXml    File (" << full_filename << ") not found" << endl;
        return false;
    }

    m_daq_config_file = full_filename;

    boost::filesystem::path p_config(full_filename);
    boost::filesystem::path dir = p_config.parent_path();

    m_map_dir = dir.string();

    cout << "Successfully found DAQ configuration file" << endl;
    cout << "    > DAQ config      : " << full_filename << endl;
    cout << "    > map directory   : " << m_map_dir << endl;

   // if(detectorConfigFile()!="") {
   //     exists = ifstream(full_det_config_name).good();
   // }
   // if(!exists) {
   //     cout << "****************************************************************************" << endl;
   //     cout << "DaqConfiguration::loadDaqXml    ERROR detector config file does not exists: "
   //          << full_det_name << endl;
   //     cout << "****************************************************************************" << endl;
   //     return false;
   // }

   // cout << "Successfully found DAQ configuration files" << endl;
   // cout << "    > detector config : " << full_det_config_name << endl;
   // cout << "    > map directory   : " << m_map_dir << endl;

   // //update filenames to have the full path
   // m_detectorConfigFile = full_det_config_name;

    return true;
}

bool DaqConfiguration::loadDetectorSetup()
{
    bool ok = true;
    cout << "DaqConfiguration::loadDetectorSetup" << endl;
    m_detConfig = new DetectorConfig();
    m_detConfig->setMapDir(m_map_dir);
    ok = m_detConfig->loadDetectorSetup(m_daq_config_file);
    return ok;
}
