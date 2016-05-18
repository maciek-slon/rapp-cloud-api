#ifndef RAPP_CLOUD_SPEECH_DETECTION_SPHINX4
#define RAPP_CLOUD_SPEECH_DETECTION_SPHINX4

#include <string>

#include <rapp/objects/header/header.hpp>

#include <boost/property_tree/ptree.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

#include <rapp/objects/picture/picture.hpp>


namespace rapp {
namespace cloud {
    
namespace ro = rapp::object;

std::string speech_detection_sphinx4(
        const std::string & fname,
        const std::string & language,
        const std::vector<std::string> & words,
        const std::vector<std::string> & grammar,
        const std::vector<std::string> & sentences,
        const std::string & audio_source,
        const std::string & user,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    std::string path = upload_file("audio.ogg", fname, host, port, debug);
    
    pt::ptree args;
    args.put("language", language, s());
    args.put("path", path, s());
    args.put("audio_source", audio_source, s());
    args.put("user", user, s());
    args.add_child("words", dump(words));
    args.add_child("grammar", dump(grammar));
    args.add_child("sentences", dump(sentences));
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_speech_detection_sphinx4/batch_speech_to_text", args, host, port, debug);
    if (res.empty())
        return "dupa";
    else    
        return res.get<std::string>("error");
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_SPEECH_DETECTION_SPHINX4 */
