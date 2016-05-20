#ifndef RAPP_CLOUD_PATH_PLANNING_PLAN
#define RAPP_CLOUD_PATH_PLANNING_PLAN

#include <string>

#include <rapp/objects/header/header.hpp>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

namespace rapp {
namespace cloud {
    
namespace ro = rapp::object;

std::vector<ro::pose_stamped> path_planning_plan(
        const std::string & user,
        const std::string & map,
        const std::string & robot,
        const std::string & algorithm,
        const ro::pose_stamped & start,
        const ro::pose_stamped & goal,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;
    
    pt::ptree args;
    args.put("user_name", user, s());
    args.put("map_name", map, s());
    args.put("robot_type", robot, s());
    args.put("algorithm", algorithm, s());
    args.add_child("start", dump(start));
    args.add_child("goal", dump(goal));
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_path_planning/planPath2d", args, host, port, debug);
    if (res.empty())
        throw std::runtime_error("Error calling path_planning_plan service");
    
    std::vector<ro::pose_stamped> path;
    
    for (auto ul : res.get_child("path")) {
        path.push_back(extract<ro::pose_stamped>(ul.second.get_child("")));
    }
    
    return path;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_PATH_PLANNING_PLAN */
