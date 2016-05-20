#ifndef RAPP_CLOUD_FACE_RECOGNITION
#define RAPP_CLOUD_FACE_RECOGNITION

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
# Learn the face model
bool learn
# CSV file's URL
string fn_csv
# Recognize faces
bool recognize
# Face model's URL
string model_name
# Container for detected face positions
geometry_msgs/PointStamped[] faces_up_left
geometry_msgs/PointStamped[] faces_down_right
# User name
string user
---

string model_name
int32[] recognizedIDs
float64[] predictedConfidenceVec
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

std::vector<int> face_recognition (
        const std::string & user,
        rapp::object::picture::Ptr pic,
        const std::string & model,
        const std::vector<ro::face> & faces,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;
    ro::header header(0, ro::time(0, 0), "frame_id");

    std::string name = upload_file(std::string("face.") + pic->type(), pic, host, port, false);
    
    pt::ptree args;
    args.put("user", user, s());
    args.add_child("header", dump(header));
    args.put("imageFilename", name, s());
    args.put("learn", false);
    args.put("fn_csv", "", s());
    args.put("recognize", true);
    args.put("model_name", model, s());
   
    pt::ptree ul, dr;
    for (auto f : faces) {
        pt::ptree ps;
        ps.add_child("header", dump(header));
        ps.add_child("point", dump(ro::point(f.ux(), f.uy(), 0)));

        ul.push_back(std::make_pair("", ps));

        pt::ptree ps2;
        ps2.add_child("header", dump(header));
        ps2.add_child("point", dump(ro::point(f.dx(), f.dy(), 0)));

        dr.push_back(std::make_pair("", ps2));
    }
    if (faces.empty()) {
        args.put("faces_up_left", "[]");
        args.put("faces_down_right", "[]");
    } else {
        args.add_child("faces_up_left", ul);
        args.add_child("faces_down_right", dr);
    }
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_face_recognition/recognize_faces", args, host, port, debug);
    if (res.empty())
        return {};

    std::vector<int> ids;
    
    for (auto ul : res.get_child("recognizedIDs")) {
        ids.push_back(ul.second.get_child("").get_value<int>());
    }
    
    return ids;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_FACE_RECOGNITION */
