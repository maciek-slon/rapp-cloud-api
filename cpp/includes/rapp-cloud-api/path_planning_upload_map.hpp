#ifndef RAPP_PATH_PLANNING_UPLOAD_MAP
#define RAPP_PATH_PLANNING_UPLOAD_MAP

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>

namespace rapp {
namespace cloud {

int path_planning_upload_map(
        const std::string & yaml_name, 
        const std::string & png_name, 
        const std::string & user_name, 
        const std::string & map_name,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;
    
    std::string base_path = "~/maps/";
    base_path += user_name + "/" + map_name + ".";
    
    upload_file(base_path+"png", png_name, host, port, debug);
    upload_file(base_path+"yaml", yaml_name, host, port, debug);
    
    return 0;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_PATH_PLANNING_UPLOAD_MAP */
