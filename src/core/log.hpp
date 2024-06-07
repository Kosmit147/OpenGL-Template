#pragma once

template<typename... Args> inline auto log_notification(Args&&... args) -> void
{
    std::println(std::forward<Args>(args)...);
};

template<typename... Args> inline auto log_warning(Args&&... args) -> void
{
    std::println(std::forward<Args>(args)...);
}

template<typename... Args> inline auto log_error(Args&&... args) -> void
{
    std::println(stderr, std::forward<Args>(args)...);
}
