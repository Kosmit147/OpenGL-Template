#pragma once

namespace ansi_colors {
static const char* red = "\x1b[31m";
static const char* yellow = "\x1b[33m";
static const char* white = "\x1b[37m";
static const char* reset = "\x1b[0m";
} // namespace ansi_colors

template<typename... Args>
inline auto log_notification(std::format_string<Args...>&& format, Args&&... args) -> void
{
    std::println(std::forward<std::format_string<Args...>>(format), std::forward<Args>(args)...);
};

template<typename... Args>
inline auto log_warning(std::format_string<Args...>&& format, Args&&... args) -> void
{
    std::cout << ansi_colors::yellow;
    std::println(std::forward<std::format_string<Args...>>(format), std::forward<Args>(args)...);
    std::cout << ansi_colors::reset;
}

template<typename... Args> inline auto log_error(std::format_string<Args...>&& format, Args&&... args) -> void
{
    std::cout << ansi_colors::red;
    std::println(stderr, std::forward<std::format_string<Args...>>(format), std::forward<Args>(args)...);
    std::cout << ansi_colors::reset;
}
