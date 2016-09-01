#ifndef DAQ_CONFIG_H
#define DAQ_CONFIG_H

/////////////////////////////////////////////////
//
// daq_config
//
// Interface between the map handler tool 
// (c.f. map_handler.h) and the detector setup
// (c.f. det_config.h)
//
// daniel.joseph.antrim@cern.ch
// University of California, Irvine
// September 2016
//
/////////////////////////////////////////////////

//std/stl
#include <vector>
#include <string>

//nsw
class DetectorConfig;

class DaqConfiguration {

    public :
        DaqConfiguration();
        virtual ~DaqConfiguration(){};

        void setDebug(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        bool loadDaqXml(std::string filename);
        std::string mapDir() { return m_map_dir; }
        std::string daqConfigFile() { return m_daq_config_file; }

        bool loadDetectorSetup();

        DetectorConfig& detConfig() { return *m_detConfig; }

    private :
        bool m_dbg;
        std::string m_map_dir;
        std::string m_daq_config_file;

        DetectorConfig* m_detConfig;


}; // class

#endif
