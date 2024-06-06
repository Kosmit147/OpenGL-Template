#pragma once

#include <filesystem>
#include <sstream>
#include <stdexcept>

// throws FileIoError
[[nodiscard]] auto read_from_file(const std::filesystem::path& path) -> std::stringstream;

class FileIoError : public std::runtime_error
{
protected:
    inline FileIoError(const char* message) noexcept : std::runtime_error(message) {}
    inline FileIoError(const std::string& message) noexcept : std::runtime_error(message) {}
    virtual inline ~FileIoError() noexcept {}
};

class InvalidFilePath : public FileIoError
{
public:
    inline InvalidFilePath(const char* message) noexcept : FileIoError(message) {}
    inline InvalidFilePath(const std::string& message) noexcept : FileIoError(message) {}
};

class FailedToOpenFile : public FileIoError
{
public:
    inline FailedToOpenFile(const char* message) noexcept : FileIoError(message) {}
    inline FailedToOpenFile(const std::string& message) noexcept : FileIoError(message) {}
};

class FailedToReadFromFile : public FileIoError
{
public:
    inline FailedToReadFromFile(const char* message) noexcept : FileIoError(message) {}
    inline FailedToReadFromFile(const std::string& message) noexcept : FileIoError(message) {}
};
