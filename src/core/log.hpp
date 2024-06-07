#pragma once

template<typename... Args>
inline auto log_notification(std::format_string<Args...>&& format, Args&&... args) -> void
{
    std::println(std::forward<std::format_string<Args...>>(format), std::forward<Args>(args)...);
};

template<typename... Args>
inline auto log_warning(std::format_string<Args...>&& format, Args&&... args) -> void
{
    std::println(std::forward<std::format_string<Args...>>(format), std::forward<Args>(args)...);
}

template<typename... Args> inline auto log_error(std::format_string<Args...>&& format, Args&&... args) -> void
{
    std::println(stderr, std::forward<std::format_string<Args...>>(format), std::forward<Args>(args)...);
}
