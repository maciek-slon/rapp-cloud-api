#ifndef RAPP_CLOUD_UTILS
#define RAPP_CLOUD_UTILS


#include <boost/property_tree/ptree.hpp>

namespace rapp {
namespace cloud {

pt::ptree create_header(const std::string & frame_id, int sec = 0, int nsec = 0, int seq = 0) {
    pt::ptree header;
    
    header.put("seq", seq);
    header.put("stamp.secs", sec);
    header.put("stamp.nsecs", nsec);
    header.put("frame_id", frame_id, s());
    
    return header;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_UTILS */
