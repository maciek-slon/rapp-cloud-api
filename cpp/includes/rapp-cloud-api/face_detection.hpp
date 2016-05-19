#ifndef RAPP_CLOUD_FACE_DETECTION
#define RAPP_CLOUD_FACE_DETECTION

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
# Contains info about time and reference
Header header
# The image's filename to perform face detection
string imageFilename
# Flag to define if a fast detection if desired
bool fast
---
# Container for detected face positions
geometry_msgs/PointStamped[] faces_up_left
geometry_msgs/PointStamped[] faces_down_right
string error
*/

/*
        face( 
          float top_left_x,
          float top_left_y,
          float bottom_right_x,
          float bottom_right_y
        )
*/

std::vector<ro::face> face_detection(
        rapp::object::picture::Ptr pic,
        bool fast,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;
    ro::header header(0, ro::time(0, 0), "frame_id");

    std::string name = upload_file(std::string("face.") + pic->type(), pic, host, port, debug);
    
    pt::ptree args;
    args.add_child("header", dump(header));
    args.put("imageFilename", name, s());
    args.put("fast", fast);
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_face_detection/detect_faces", args, host, port, debug);
    if (res.empty())
        return {};
    
    std::vector<float> ux;
    std::vector<float> uy;
    std::vector<float> bx;
    std::vector<float> by;
    std::vector<ro::face> faces;
    
    for (auto ul : res.get_child("faces_up_left")) {
        ux.push_back(ul.second.get_child("").get<float>("point.x"));
        uy.push_back(ul.second.get_child("").get<float>("point.y"));
    }
    for (auto ul : res.get_child("faces_down_right")) {
        bx.push_back(ul.second.get_child("").get<float>("point.x"));
        by.push_back(ul.second.get_child("").get<float>("point.y"));
    }
    
    for (int i = 0 ; i < ux.size(); ++i) {
        faces.push_back(ro::face(ux[i], uy[i], bx[i], by[i]));
    }
    
    return faces;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_FACE_DETECTION */
