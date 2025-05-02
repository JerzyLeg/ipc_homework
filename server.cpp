#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void session(tcp::socket &socket)
{
    uint64_t file_size;
    boost::asio::read(socket, boost::asio::buffer(&file_size, sizeof(file_size)));
    std::ofstream output("received_file", std::ios::binary);

    std::array<char, 4096> data;
    size_t total_received = 0;
    while (total_received < file_size)
    {
        size_t bytes_to_read = std::min(data.size(), file_size - total_received);
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(data.data(), bytes_to_read), error);
        if (length == 0)
            throw std::runtime_error("Connection closed by peer.");
        if (error)
            throw std::runtime_error("Read error: " + error.message());

        output.write(data.data(), length);
        total_received += length;
    }
    std::cout << "Received file of size: " << file_size << " bytes" << std::endl;
}

int main(int, char **)
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            session(socket);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
        return 1;
    }
}