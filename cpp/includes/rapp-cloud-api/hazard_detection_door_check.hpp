#ifndef RAPP_CLOUD_HAZARD_DETECTION_DOOR_CHECK
#define RAPP_CLOUD_HAZARD_DETECTION_DOOR_CHECK

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>

namespace rapp {
namespace cloud {

int hazard_detection_door_check(rapp::object::picture::Ptr pic) {
    namespace pt = boost::property_tree;

    std::string name = upload_file(std::string("door.") + pic->type(), pic);
    
    pt::ptree args;
    args.add_child("header", create_header("frame_id"));
    args.put("imageFilename", name, s());
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_hazard_detection/door_check", args, "localhost", "9090", true);
    if (res.empty())
        return -1;
    else    
        return res.get<int>("door_angle");
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_HAZARD_DETECTION_DOOR_CHECK */
