#ifndef RAPP_CLOUD_UTILS
#define RAPP_CLOUD_UTILS


#include <boost/property_tree/ptree.hpp>

#include <rapp/objects/time/time.hpp>
#include <rapp/objects/header/header.hpp>
#include <rapp/objects/point/point.hpp>
#include <rapp/objects/quaternion/quaternion.hpp>
#include <rapp/objects/pose/pose.hpp>
#include <rapp/objects/pose_stamped/pose_stamped.hpp>

namespace rapp {
namespace cloud {

namespace ro = rapp::object;
namespace pt = boost::property_tree;

struct s
{
    typedef std::string internal_type;
    typedef std::string external_type;

    boost::optional<std::string> get_value(const std::string &v) { return  v.substr(1, v.size() - 2) ; }
    boost::optional<std::string> put_value(const std::string &v) { return std::string("\"") + v + std::string("\""); }
};

pt::ptree dump(const std::vector<std::string> & v) {
    pt::ptree tree;
    
    if (v.empty()) {
        tree.put("", "[]");
    }
    
    for (auto str : v) {
        pt::ptree elem;
        elem.put("", str, s());
        tree.push_back(std::make_pair("", elem));
    }
    
    return tree;
}

pt::ptree dump(const ro::time & t) {
    pt::ptree tree;
    
    tree.put("secs", t.sec);
    tree.put("nsecs", t.nsec);
    
    return tree;
}

pt::ptree dump(const ro::header & h) {
    pt::ptree tree;
    
    tree.put("seq", h.seq);
    tree.put("frame_id", h.frame_id, s());
    tree.add_child("stamp", dump(h.stamp));
    
    return tree;
}

pt::ptree dump(const ro::point & p) {
    pt::ptree tree;
    
    tree.put("x", p.x);
    tree.put("y", p.y);
    tree.put("z", p.z);
    
    return tree;
}

pt::ptree dump(const ro::quaternion & q) {
    pt::ptree tree;
    
    tree.put("x", q.x);
    tree.put("y", q.y);
    tree.put("z", q.z);
    tree.put("w", q.w);
    
    return tree;
}

pt::ptree dump(const ro::pose & p) {
    pt::ptree tree;
    
    tree.add_child("position", dump(p.position));
    tree.add_child("orientation", dump(p.orientation));
    
    return tree;
}

pt::ptree dump(const ro::pose_stamped & p) {
    pt::ptree tree;
    
    tree.add_child("header", dump(p.header));
    tree.add_child("pose", dump(p.pose));
    
    return tree;
}

template<typename T>
T extract(const pt::ptree & tree) {
    T ret;
    return ret;
}

template<>
ro::pose extract<ro::pose>(const pt::ptree & tree) {
    ro::pose ret;
    
    ret.position.x = tree.get<float>("position.x");
    ret.position.y = tree.get<float>("position.y");
    ret.position.z = tree.get<float>("position.z");
    
    ret.orientation.x = tree.get<float>("orientation.x");
    ret.orientation.y = tree.get<float>("orientation.y");
    ret.orientation.z = tree.get<float>("orientation.z");
    ret.orientation.w = tree.get<float>("orientation.w");
        
    return ret;
}

template<>
ro::header extract<ro::header>(const pt::ptree & tree) {
    ro::header ret;
    
    ret.seq = tree.get<int>("seq");
    ret.frame_id = tree.get<std::string>("frame_id");
    ret.stamp.sec = tree.get<long>("stamp.secs");
    ret.stamp.nsec = tree.get<long>("stamp.nsecs");
    
    return ret;
}

template<>
ro::pose_stamped extract<ro::pose_stamped>(const pt::ptree & tree) {
    ro::pose_stamped ret;
    
    ret.header = extract<ro::header>(tree.get_child("header"));
    ret.pose = extract<ro::pose>(tree.get_child("pose"));
    
    return ret;
}


} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_UTILS */
