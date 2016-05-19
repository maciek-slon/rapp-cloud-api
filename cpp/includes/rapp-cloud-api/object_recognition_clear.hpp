#ifndef RAPP_CLOUD_OBJECT_RECOGNITION_CLEAR
#define RAPP_CLOUD_OBJECT_RECOGNITION_CLEAR

#include <string>

#include <rapp/objects/header/header.hpp>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>
#include <rapp/objects/face/face.hpp>


namespace rapp {
namespace cloud {
    
namespace ro = rapp::object;

/*
# User name
string user
# Object names to load
string[] names
---
# Result: 0 - ok, -1 - no models, -2 - no image to analyse
int32 result
*/

bool object_recognition_clear (
        const std::string & user,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    pt::ptree args;
    args.put("user", user, s());
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_object_recognition/clear_models", args, host, port, debug);
    if (res.empty())
        return false;
    
    return true;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_OBJECT_RECOGNITION_CLEAR */
