//nsw
#include "connector.h"

//std/stl
#include <iostream>
#include <sstream>
using namespace std;

//boost
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
// Connector
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////
Connector::Connector() :
    m_dbg(false),
    m_id(-1),
    m_map_dir(""),
    m_type(""),
    m_element_type(""),
    m_elx_map(""),
    m_chamber_map(""),
    m_board_loaded(false)
{
}
// ------------------------------------------------------------------------ //
bool Connector::load(const boost::property_tree::ptree::value_type pt)
{
    bool ok = true;
    using boost::property_tree::ptree;
    using namespace boost;

    try {

        BOOST_FOREACH(const ptree::value_type &v, pt.second) {

            ///////////////////////////////////////////////
            // type (MM or TGC)
            ///////////////////////////////////////////////
            if(v.first == "type") {
                m_type = v.second.data();
                trim(m_type);
            }
            ///////////////////////////////////////////////
            // element type (strips, pads, wires)
            ///////////////////////////////////////////////
            else if(v.first == "element_type") {
                m_element_type = v.second.data();
                trim(m_element_type);
            }
            ///////////////////////////////////////////////
            // maps
            ///////////////////////////////////////////////
            else if(v.first == "electronics_map") {
                string elx_map = v.second.data();
                trim(elx_map);
                if(!checkMapFile(elx_map, "electronics_map")) {
                    ok = false;
                    break;
                }
                m_elx_map = elx_map;
            }
            else if(v.first == "chamber_map") {
                string chamber_map = v.second.data();
                trim(chamber_map);
                if(!checkMapFile(chamber_map, "chamber_map")) {
                    ok = false;
                    break;
                }
                m_chamber_map = chamber_map; 
            }

        } // foreach

    } // try
    catch(std::exception &e) {
        cout << "Connector::load    ERROR " << e.what() << endl;
        ok = false;
    } // catch


    if(ok) {
        try {

            BOOST_FOREACH(const ptree::value_type &v, pt.second) {

                ///////////////////////////////////////////////
                // load board at this connector
                // (should only be one board per connector)
                ///////////////////////////////////////////////
                if(v.first == "board") {
                    if(boardLoaded()) {
                        cout << "Connector::load    ERROR Attempting to load more than 1 board at "
                             << "Connector with (id, type, element_type) = ("
                             << id() << ", " << type() << ", " << elementType() << ")" << endl; 
                        ok = false;
                        break;
                    }
                    Board tmpBoard;
                    tmpBoard.setDebug(m_dbg);
                    if(!tmpBoard.load(v)) ok = false;
                    if(ok) m_board = tmpBoard;
                }


            } // foreach

        } // try
        catch(std::exception &e) {
            cout << "Connector::load    ERROR " << e.what() << endl;
            ok = false;

        } // catch

    } // ok

    return ok;
}
// ------------------------------------------------------------------------ //
bool Connector::checkMapFile(string mapfilename, std::string mapnode)
{
    stringstream full_name;
    full_name << m_map_dir;
    string suffix = "/";
    if(!(boost::algorithm::ends_with(m_map_dir, suffix))) full_name << "/";
    full_name << mapfilename;

    string map_file = full_name.str();

    bool exists = std::ifstream(map_file).good();
    if(!exists) {
        cout << "Connector::checkMapFile    ERROR " << mapnode << " (" << map_file << ") unable to be loaded" << endl;
    }
    return exists;
}
