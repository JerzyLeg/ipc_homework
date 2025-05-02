#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

uint64_t getFileSize(const std::string &filename)
{
    return std::filesystem::file_size(filename);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: Missing required argument.\n";
        return 1;
    }

    std::string filename = argv[1];
    try
    {

        boost::asio::io_context io_context;
        tcp::socket socket(io_context);

        socket.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 8080));

        uint64_t file_size = getFileSize(filename);
        std::cout << "Sending file: " << filename << " (" << file_size << " bytes)" << std::endl;
        boost::asio::write(socket, boost::asio::buffer(&file_size, sizeof(file_size)));

        std::ifstream file(filename, std::ios::binary);
        if (!file)
            throw std::runtime_error("Failed to open file for reading.");

        std::vector<char> buffer(4096);

        while (!file.eof())
        {
            file.read(buffer.data(), buffer.size());
            size_t bytes_read = file.gcount();
            if (bytes_read > 0)
            {
                boost::asio::write(socket, boost::asio::buffer(buffer.data(), bytes_read));
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
        return 1;
    }
}