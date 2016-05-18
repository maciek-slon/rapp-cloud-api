#ifndef RAPP_CLOUD_HAZARD_DETECTION_DOOR_CHECK
#define RAPP_CLOUD_HAZARD_DETECTION_DOOR_CHECK

#include <string>

#include <rapp/objects/header/header.hpp>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>


namespace rapp {
namespace cloud {
    
namespace ro = rapp::object;

int hazard_detection_door_check(
        rapp::object::picture::Ptr pic,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;
    ro::header header(0, ro::time(0, 0), "frame_id");

    std::string name = upload_file(std::string("door.") + pic->type(), pic, host, port, debug);
    
    pt::ptree args;
    args.add_child("header", dump(header));
    args.put("imageFilename", name, s());
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_hazard_detection/door_check", args, host, port, debug);
    if (res.empty())
        return -1;
    else    
        return res.get<int>("door_angle");
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_HAZARD_DETECTION_DOOR_CHECK */
