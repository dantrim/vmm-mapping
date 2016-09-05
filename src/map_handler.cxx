//nsw
#include "map_handler.h"
#include "daq_config.h"
#include "det_config.h"
#include "element_type.h"

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
// ------------------------------------------------------------------------ //
bool MapHandler::buildMapping()
{
    bool ok = true;

    if(!m_initialized) {
        cout << "MapHandler::buildMapping    Detector setup has not yet been loaded (MapHandler::loadDaqConfiguration). Cannot build mapping!" << endl;
        return false;
    }

    int n_element_loaded = 0;
    // loop over boards
    nsw_map::febIdToChannelMap id_map;

    for(auto& boardMap : m_daqConfig->boardMap()) {
        int boardId = boardMap.second;
        string boardIp = boardMap.first;

        nsw_map::febChanToElementMap element_map;

        for(auto& detector : m_daqConfig->detConfig().detectors()) {
            if(!detector.hasBoardId(boardId)) continue;

            for(auto& module : detector.modules()) {
                if(!module.hasBoardId(boardId)) continue;

                for(auto& layer : module.layers()) {
                    if(!layer.hasBoardId(boardId)) continue;

                    for(auto& connector : layer.connectors()) {
                        if(!connector.hasBoardId(boardId)) continue;


                        // fill the contents for building up the monitoring tree
                        nsw_map::boardContentMap tmpMap;
                        vector<string> vmm_names;
                        for(auto chip : connector.board().chips()) {
                            vmm_names.push_back(chip.name());
                        }
                        tmpMap[connector.board().name()] = vmm_names;
                        m_board_contents.push_back(tmpMap);

                        // loop over VMM
                        for(auto& vmm : connector.elxMap()) {
                            int vmm_id = std::get<0>(vmm);
                            int vmm_chan = std::get<1>(vmm);
                            int feb_chan = std::get<2>(vmm);

                            // now get strip for this (VMM id, VMM chan) combo
                            for(auto& element : connector.chamberMap()) {
                                if(!(feb_chan==std::get<0>(element))) continue;

                                int elem_type = std::get<1>(element);
                                int element_number = std::get<2>(element);

                                nsw_map::ElementType element_type = nsw_map::ElementTypeFromInt(elem_type);
                                if(element_type == nsw_map::ElementType::ElementTypeInvalid) {
                                    cout << "MapHandler::buildMapping    ERROR Invalid element type ("
                                         << elem_type << ") found "
                                         << "in mapping for (board id, vmm id, vmm chan, feb chan) = ("
                                         << boardId << ", " << vmm_id << ", " << vmm_chan << ", " << feb_chan
                                         << ")" << endl;
                                    cout << "MapHandler::buildMapping    ERROR >>> Map building aborted!" << endl;
                                    m_map_loaded = false;
                                    return false;
                                }
                                string element_type_str = nsw_map::ElementType2Str(element_type);

                                /////////////////////////////////////////////////
                                // build up the Element object for this element
                                /////////////////////////////////////////////////
                                Element elemObj;

                                elemObj.init(n_element_loaded);

                                elemObj.setBoardIp(boardIp);
                                elemObj.setBoardId(boardId);
                                elemObj.setBoardName(connector.board().name());
                                elemObj.setBoardType(connector.board().type());
                                elemObj.setChipName(connector.board().chipNameFromId(vmm_id));
                                elemObj.setElementType(elem_type);
                                elemObj.setDetectorId(detector.id());
                                elemObj.setModuleId(module.id());
                                elemObj.setLayerId(layer.id());
                                elemObj.setConnectorId(connector.id());

                                elemObj.setVMMId(vmm_id);
                                elemObj.setVMMChan(vmm_chan);
                                elemObj.setFEBChan(feb_chan);
                                elemObj.setElementNumber(element_number);

                                element_map[feb_chan] = elemObj;

                                n_element_loaded++;
                            } // element


                        } // vmm

                    } // connector
                } // layer
            } // module
        } // detector

        id_map[boardId] = element_map;

    } // board

    // this is the final map
    m_daq_map = id_map;

    //cout << m_daq_map[0][166].vmmId() << "  " << m_daq_map[0][166].vmmChan() << "  " << m_daq_map[0][166].elementNumber() << "  " << m_daq_map[0][166].boardType() << "  " << m_daq_map[0][166].chipName() << "  " << m_daq_map[0][166].elementType() << "  " << m_daq_map[0][166].elementTypeStr() << endl;


    return ok;
}
