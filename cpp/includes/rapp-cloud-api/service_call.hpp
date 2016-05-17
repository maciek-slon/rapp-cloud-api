#ifndef RAPP_SERVICE_CALL
#define RAPP_SERVICE_CALL

#include <iostream>
#include <iterator>
#include <sstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/property_tree/ptree.hpp>
#include "json_parser.hpp"

namespace rapp {
namespace cloud {

namespace pt = boost::property_tree;

using boost::asio::ip::tcp;

struct s
{
    typedef std::string internal_type;
    typedef std::string external_type;

    boost::optional<std::string> get_value(const std::string &v) { return  v.substr(1, v.size() - 2) ; }
    boost::optional<std::string> put_value(const std::string &v) { return std::string("\"") + v + std::string("\""); }
};


std::string base64_encode(const std::vector<char> & data) {
    const std::string base64_padding[] = {"", "==","="};
  
    using namespace boost::archive::iterators;
    typedef std::vector<char>::const_iterator iterator_type;
    typedef base64_from_binary<transform_width<iterator_type, 6, 8> > base64_enc;
    std::stringstream ss;
    std::copy(base64_enc(data.begin()), base64_enc(data.end()), std::ostream_iterator<char>(ss));
    ss << base64_padding[data.size() % 3];
    return ss.str();
}

pt::ptree service_call(const std::string & name, const pt::ptree & args, const std::string & host = "localhost", const std::string & port = "9090", bool debug = false) {
 
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    boost::array<char, 10240> buf;
    boost::system::error_code error;

    pt::ptree tree;

    tree.put("op", "call_service", s());
    tree.put("service", name, s());
    tree.add_child("args", args);
    
    std::stringstream ss;
    pt::write_json(ss, tree);
    if (debug) {        
        std::cout << "Full json: " << ss.str() << std::endl;
    }
            
    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(ss.str()), boost::asio::transfer_all(), ignored_error);

    size_t len = socket.read_some(boost::asio::buffer(buf), error);

    if (error == boost::asio::error::eof)
        std::cout << "EOF\n";
    else if (error)
        throw boost::system::system_error(error); // Some other error.

    if (debug) {
        std::cout.write(buf.data(), len);
        std::cout << std::endl;
    }
    
    pt::ptree tree_out;
    std::istringstream is(std::string(buf.begin(), buf.begin()+len));
    pt::read_json(is, tree_out);
    pt::ptree res;
    
    if (tree_out.get<bool>("result")) {
        
        res = tree_out.get_child("values");
        if (debug) {
            std::stringstream ss;
            pt::write_json(ss, res);
            std::cout << "Result json: " << ss.str() << std::endl;
        }
        
    } else {
        std::cout << tree_out.get<std::string>("values") << std::endl;
    }
    
    return res;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_SERVICE_CALL */
