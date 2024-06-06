#include "file_io.hpp"

#include <fstream>

auto read_from_file(const std::filesystem::path& path) -> std::stringstream
{
    if (!std::filesystem::exists(path)) [[unlikely]]
    {
        auto message = std::format("Invalid file path: {}", path.string());
        throw InvalidFilePath{ message };
    }

    std::ifstream file(path);
    std::stringstream stream;

    auto file_buf = file.rdbuf();

    if (!file_buf) [[unlikely]]
    {
        auto message = std::format("Can't open file: {}", path.string());
        throw FailedToOpenFile{ message };
    }

    stream << file_buf << '\0';

    if (!stream.good()) [[unlikely]]
    {
        auto message = std::format("Can't read from file: {}", path.string());
        throw FailedToReadFromFile{ message };
    }

    return stream;
}
