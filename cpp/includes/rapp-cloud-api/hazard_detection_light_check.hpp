#ifndef RAPP_CLOUD_HAZARD_DETECTION_LIGHT_CHECK
#define RAPP_CLOUD_HAZARD_DETECTION_LIGHT_CHECK

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>

namespace rapp {
namespace cloud {

int hazard_detection_light_check(rapp::object::picture::Ptr pic) {
    namespace pt = boost::property_tree;

    std::string name = upload_file(std::string("light.") + pic->type(), pic);
    
    pt::ptree args;
    args.add_child("header", create_header("frame_id"));
    args.put("imageFilename", name, s());
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_hazard_detection/light_check", args);
    if (res.empty())
        return -1;
    else    
        return res.get<int>("light_level");
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_HAZARD_DETECTION_LIGHT_CHECK */
