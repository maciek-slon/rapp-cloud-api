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

#include "utils.hpp"

namespace rapp {
namespace cloud {

namespace pt = boost::property_tree;

using boost::asio::ip::tcp;

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

std::string base64_encode(std::istream & data) {
    const std::string base64_padding[] = {"", "==","="};
  
    using namespace boost::archive::iterators;
    typedef std::istream_iterator<char> iterator_type;
    typedef base64_from_binary<transform_width<iterator_type, 6, 8> > base64_enc;
    std::stringstream ss;
    data.seekg(0, std::ios_base::end);
    std::streampos fileSize = data.tellg();
    data.seekg(0, std::ios_base::beg);
    std::copy(base64_enc(iterator_type(data)), base64_enc(iterator_type()), std::ostream_iterator<char>(ss));
    ss << base64_padding[fileSize % 3];
    return ss.str();
}

pt::ptree service_call(const std::string & name, const pt::ptree & args, const std::string & host = "localhost", const std::string & port = "9090", bool debug = false) {
 
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    
    
    boost::asio::connect(socket, endpoint_iterator);
    //socket.set_option( boost::asio::ip::tcp::no_delay( true ) );
    //socket.set_option( boost::asio::socket_base::send_buffer_size( 1024000 ) );
    //socket.set_option( boost::asio::socket_base::receive_buffer_size( 1024000 ) );



    boost::array<char, 1024000> buf;
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

    pt::ptree tree_out;
    bool success = false;

    std::string json_reply;
    std::string jtmp;

    do {
        size_t len;
        
        len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof) {
            std::cout << "EOF\n";
            break;
        }
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        if (debug) {
            std::cout << "Returned: " << len << " bytes =================================\n";
            std::cout.write(buf.data(), len);
            std::cout << std::endl;
        }
        
        try {
            json_reply += std::string(buf.begin(), buf.begin()+len);
            if (json_reply.back() == '}') {
                jtmp = json_reply;
                std::istringstream is(jtmp);
                pt::read_json(is, tree_out);
                success = true;
            }
        }
        catch (std::exception & e) {
            if (debug) std::cout << e.what() << "\n";
        }
        
    } while(!success);
    
    pt::ptree res;
    if (tree_out.get<bool>("result")) {
        
        res = tree_out.get_child("values");
        if (debug) {
            std::stringstream ss;
            pt::write_json(ss, res);
            //std::cout << "Result json: " << ss.str() << std::endl;
        }
        
    } else {
        //std::cout << tree_out.get<std::string>("values") << std::endl;
    }
    
    return res;
}

} // namespace cloud
} // namespace rapp

#endif /* RAPP_SERVICE_CALL */
