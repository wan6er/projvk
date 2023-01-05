#include "utils/file.h"

#include <fstream>

namespace utils
{

std::vector<char> load_file(std::string CONST_REFERENCE path)
{
    // std::ifstream file(path, std::ios::ate | std::ios::binary);
    // if (!file.is_open()) {
    //     throw std::runtime_error("failed to open file!");
    // }
    // size_t fileSize = (size_t) file.tellg();
    // std::vector<char> buffer(fileSize);
    // file.seekg(0);
    // file.read(buffer.data(), fileSize);
    // file.close();

    // auto f = fopen(path.c_str(), "r");
    // fseek(f, 0, SEEK_END);
    // size_t size = ftell(f);
    // fseek(f, 0, SEEK_SET);
    // std::vector<char> buffer(size);
    // fread(buffer.data(), sizeof(char), size, f);
    // return buffer;

    std::ifstream is(path, std::ios::binary | std::ios::in | std::ios::ate);
    if (is.is_open())
    {
        size_t size = is.tellg();
        is.seekg(0, std::ios::beg);
        // char* shaderCode = new char[size];
        std::vector<char> file(size);
        is.read(file.data(), size);
        is.close();
        return file;
    }
    return {};
}
    
} // namespace utils
