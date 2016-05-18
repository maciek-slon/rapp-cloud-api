#ifndef RAPP_CLOUD_UPLOAD_FILE
#define RAPP_CLOUD_UPLOAD_FILE

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>

#include <rapp/objects/picture/picture.hpp>

namespace rapp {
namespace cloud {

std::string upload_file(
        const std::string & name, 
        rapp::object::picture::Ptr pic, 
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    pt::ptree args;
    args.put("data", rapp::cloud::base64_encode(pic->bytearray()), s());
    args.put("name", name, s());
    
    pt::ptree res = rapp::cloud::service_call("/upload_file", args, host, port, debug);
    if (res.empty())
        return "";
    else    
        return res.get<std::string>("path");
}

std::string upload_file(
        const std::string & name, 
        const std::string & fname,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    std::vector<char> bytearray_;
    std::ifstream bytestream(fname, std::ios::in | std::ios::binary | std::ios::ate);
    
    // copy byte by byte
    bytestream.seekg(0, std::ios_base::end);
    std::streampos fileSize = bytestream.tellg();
    
    std::cout << "File " << fname << " size: " << fileSize << "\n";
    bytearray_.resize(fileSize);
    bytestream.seekg(0, std::ios_base::beg);
    bytestream.read(&bytearray_[0], fileSize);


    pt::ptree args;
    args.put("data", rapp::cloud::base64_encode(bytearray_), s());
    args.put("name", name, s());
    
    pt::ptree res = rapp::cloud::service_call("/upload_file", args, host, port, debug);
    if (res.empty())
        return "";
    else    
        return res.get<std::string>("path");
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_UPLOAD_FILE */
