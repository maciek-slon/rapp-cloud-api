#ifndef RAPP_CLOUD_UPLOAD_FILE
#define RAPP_CLOUD_UPLOAD_FILE

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>

#include <rapp/objects/picture/picture.hpp>

namespace rapp {
namespace cloud {

std::string upload_file(const std::string & name, rapp::object::picture::Ptr pic) {
    namespace pt = boost::property_tree;

    pt::ptree args;
    args.put("data", rapp::cloud::base64_encode(pic->bytearray()), s());
    args.put("name", name, s());
    
    pt::ptree res = rapp::cloud::service_call("/upload_image", args);
    if (res.empty())
        return "";
    else    
        return res.get<std::string>("path");
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_UPLOAD_FILE */
