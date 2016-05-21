#ifndef RAPP_CLOUD_SEND_EMAIL
#define RAPP_CLOUD_SEND_EMAIL

#include <string>

#include <rapp/objects/header/header.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>

#include <rapp-cloud-api/service_call.hpp>
#include <rapp-cloud-api/utils.hpp>
#include <rapp-cloud-api/upload_file.hpp>

namespace rapp {
namespace cloud {
    
namespace ro = rapp::object;

/*
# The user email username
string userEmail
# The user email password
string password
# The email server's smtp address, i.e. 'smtp.gmail.com'
string server
# The email server smtp port
string port

# Email addresses of the recipients
string[] recipients

# The email body
string body
# The email subject
string subject

# File paths of the attachments
string[] files

---
# Response

# 0 success, -1 failure
int8 status
*/

bool send_email (
        const std::string & user,
        const std::string & pass,
        const std::string & server,
        const std::string & mport,
        const std::vector<std::string> & recipients,
        const std::string & subject,
        const std::string & body,
        const std::vector<std::string> & files,
        const std::string & host = "localhost",
        const std::string & port = "9090",
        bool debug = false
    ) {
    namespace pt = boost::property_tree;

    std::vector<std::string> files_v;
    for (const auto & f : files) {
        boost::filesystem::path p(f);
        files_v.push_back(rapp::cloud::upload_file(p.filename().string(), f, host, port, false));
    }

    pt::ptree args;
    args.put("userEmail", user, s());
    args.put("password", pass, s());
    args.put("server", server, s());
    args.put("port", mport, s());
    args.add_child("recipients", dump(recipients));
    args.put("subject", subject, s());
    args.put("body", body, s());
    args.add_child("files", dump(files_v));
    
    pt::ptree res = rapp::cloud::service_call("/rapp/rapp_email_send/send_email", args, host, port, debug);
    if (res.empty())
        return false;
    
    return (res.get("status", -1) >= 0);
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_CLOUD_SEND_EMAIL */
