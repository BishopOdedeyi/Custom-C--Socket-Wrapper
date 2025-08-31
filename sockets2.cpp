#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <boost/asio/ip/host_name.hpp>

class Socket {
public:
    enum Protocol { TCP, UDP };

    Socket(boost::asio::io_context& io_context, int family, Protocol type, int protocol)
        : io_context_(io_context), type_(type) {
        if (type_ == TCP) {
            tcp_socket_ = std::make_unique<boost::asio::ip::tcp::socket>(io_context_);
        } else {
            udp_socket_ = std::make_unique<boost::asio::ip::udp::socket>(io_context_);
        }
    }

    void bind(const std::string& address, unsigned short port) {
        if (type_ == TCP) {
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(address), port);
            tcp_acceptor_ = std::make_unique<boost::asio::ip::tcp::acceptor>(io_context_, endpoint);
        } else {
            boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::make_address(address), port);
            udp_socket_->open(endpoint.protocol());
            udp_socket_->bind(endpoint);
        }
    }

    void connect(const std::string& address, unsigned short port) {
        if (type_ == TCP) {
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
            boost::asio::connect(*tcp_socket_, endpoints);
        } else {
            throw std::runtime_error("UDP sockets do not support connect in the same way as TCP.");
        }
    }

    void listen(int backlog = 5) {
        if (type_ == TCP && tcp_acceptor_) {
            tcp_acceptor_->listen(backlog);
        } else {
            throw std::runtime_error("UDP sockets do not support listen.");
        }
    }

    void close() {
        if (type_ == TCP && tcp_socket_) {
            tcp_socket_->close();
        } else if (udp_socket_) {
            udp_socket_->close();
        }
    }

    std::string gethostname() {
        return boost::asio::ip::host_name();
    }

    std::string gethostbyname(const std::string& hostname) {
        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::ip::tcp::resolver::results_type results = resolver.resolve(hostname, "");
        return results.begin()->endpoint().address().to_string();
    }

    void send(const std::string& message) {
        if (type_ == TCP && tcp_socket_) {
            boost::asio::write(*tcp_socket_, boost::asio::buffer(message));
        } else {
            throw std::runtime_error("Send is only supported for TCP sockets.");
        }
    }

    std::string recv(std::size_t buffer_size) {
        if (type_ == TCP && tcp_socket_) {
            std::array<char, 1024> buffer;
            std::size_t len = tcp_socket_->read_some(boost::asio::buffer(buffer, buffer_size));
            return std::string(buffer.data(), len);
        } else {
            throw std::runtime_error("Recv is only supported for TCP sockets.");
        }
    }

private:
    boost::asio::io_context& io_context_;
    Protocol type_;
    std::unique_ptr<boost::asio::ip::tcp::socket> tcp_socket_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> tcp_acceptor_;
    std::unique_ptr<boost::asio::ip::udp::socket> udp_socket_;
};

int main() {
    boost::asio::io_context io_context;
    Socket tcp_server(io_context, AF_INET, Socket::TCP, 0);
    tcp_server.bind("127.0.0.1", 8080);
    tcp_server.listen();
    std::cout << "TCP server listening on port 8080" << std::endl;
    
    std::cout << "Hostname: " << tcp_server.gethostname() << std::endl;
    std::cout << "Google IP: " << tcp_server.gethostbyname("google.com") << std::endl;
    
    return 0;
}