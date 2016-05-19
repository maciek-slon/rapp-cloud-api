#ifndef RAPP_CLOUD_OBJECT_RECOGNITION_LEARN
#define RAPP_CLOUD_OBJECT_RECOGNITION_LEARN

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
# Path (id) of model image
string fname
# Name of the object
string name
# User name
string user
---
# Result: 0 - ok, -1 - no models, -2 - no image to analyse
int32 result
*/

bool object_recognition_learn(
        const std::string & user,
        rapp::object::picture::Ptr pic,
        const std::string & name,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    std::string base_path = "~/";
    base_path += user + "/models/";
    std::string fname = upload_file(base_path + name + "." + pic->type(), pic, host, port, false);
        
    pt::ptree args;
    args.put("fname", fname, s());
    args.put("name", name, s());
    args.put("user", user, s());
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_object_recognition/learn_object", args, host, port, debug);
    if (res.empty())
        return false;
    
    int retcode = res.get<int>("result");
    
    switch(retcode) {
        case 0: return true;
        case -1: std::cout << "no models\n"; return false;
        case -2: std::cout << "no image\n"; false;
    }
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_OBJECT_RECOGNITION_LEARN */
