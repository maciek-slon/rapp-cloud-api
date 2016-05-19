#ifndef RAPP_CLOUD_OBJECT_RECOGNITION_DETECT
#define RAPP_CLOUD_OBJECT_RECOGNITION_DETECT

#include <string>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>
#include <rapp/objects/object/object.hpp>


namespace rapp {
namespace cloud {
    
namespace ro = rapp::object;

/*
# Path (id) of query image
string fname
# List of known objects - names
string[] names
# List of known objects - image files (models)
string[] files
# Limit search to N best matches
uint32 limit
# User name
string user
---
# List of found objects - names
string[] found_names
# List of found objects - centroids in image
geometry_msgs/Point[] found_centers
# List of found objects - scores
float64[] found_scores
# Result: 0 - ok, -1 - no models, -2 - no image to analyse
int32 result
*/

std::vector<ro::object> object_recognition_detect(
        const std::string & user,
        rapp::object::picture::Ptr pic,
        const std::vector<std::string> & names,
        const std::vector<std::string> & models,
        int limit,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    std::string name = upload_file(std::string("object.") + pic->type(), pic, host, port, false);
    
    std::vector<std::string> model_files;
    for (const auto & m : models) {
        model_files.push_back(upload_file("model", m, host, port, false));
    }
    
    pt::ptree args;
    args.put("user", user, s());
    args.put("fname", name, s());
    args.add_child("names", dump(names));
    args.add_child("files", dump(model_files));
    args.put("limit", limit);
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_object_recognition/find_objects", args, host, port, debug);
    if (res.empty())
        return {};
    
    std::vector<ro::point> found_centers;
    std::vector<float> found_scores;
    std::vector<std::string> found_names;
    
    std::vector<ro::object> ret;
    
    for (auto ul : res.get_child("found_names")) {
        found_names.push_back(ul.second.get_child("").get_value<std::string>());
    }
    for (auto ul : res.get_child("found_centers")) {
        ro::point pt = {
            ul.second.get_child("").get<float>("x"),
            ul.second.get_child("").get<float>("y"),
            0
        };
        found_centers.push_back(pt);
    }
    for (auto ul : res.get_child("found_scores")) {
        found_scores.push_back(ul.second.get_child("").get_value<float>());
    }
    
    
    for (int i = 0 ; i < found_names.size(); ++i) {
        ret.push_back(ro::object(found_names[i], found_centers[i], found_scores[i]));
    }
    
    return ret;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_OBJECT_RECOGNITION_DETECT */
