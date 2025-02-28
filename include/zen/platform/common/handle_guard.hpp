#pragma once

#include <zen/core/requirements.hpp>

namespace zen {
namespace detail {
struct nt_close_guard
{
    NODISCARD
    constexpr
    static
    auto
    invalid_value() noexcept -> const void*
    {
        return win::invalid_handle_value();
    }

    NODISCARD
    constexpr
    static
    auto
    valid(
        const void* const handle
    ) noexcept -> bool
    {
        return win::is_valid_handle(handle);
    }

    static
    auto
    close(
        const void* handle
    ) noexcept -> void;
};

template<class T>
concept close_guard = requires
{
    { T::invalid_value() } -> std::convertible_to<const void*>;
    { T::valid(std::declval<const void*>()) } -> std::convertible_to<bool>;
    { T::close(std::declval<const void*>()) } -> std::convertible_to<void>;
};
} //namespace detail

namespace win {
template<detail::close_guard CloseGuard>
class handle_guard
{
public:
    constexpr
    handle_guard() noexcept = default;

    constexpr
    explicit
    handle_guard(
        const void* const value
    ) noexcept
        : handle_{value}
    {}

    constexpr
    handle_guard(
        const handle_guard& rhs
    ) noexcept = delete;

    constexpr
    handle_guard(
        handle_guard&& rhs
    ) noexcept
        : handle_{rhs.release()}
    {}

    constexpr
    ~handle_guard() noexcept = default;

    constexpr
    auto
    operator=(
        const handle_guard& rhs
    ) noexcept -> handle_guard& = delete;

    ZEN_CXX23_CONSTEXPR
    auto
    operator=(
        handle_guard&& rhs
    ) noexcept -> handle_guard&
    {
        reset(rhs.release());

        return *this;
    }

    ZEN_CXX23_CONSTEXPR
    auto
    operator=(
        const void* const rhs
    ) noexcept -> handle_guard&
    {
        reset(rhs);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    operator==(
        const handle_guard& rhs
    ) const noexcept -> bool
    {
        return operator==(rhs.get());
    }

    NODISCARD
    constexpr
    auto
    operator==(
        const void* const rhs
    ) const noexcept -> bool
    {
        return get() == rhs;
    }

    NODISCARD
    constexpr
    auto
    operator!=(
        const handle_guard& rhs
    ) const noexcept -> bool
    {
        return !(*this == rhs);
    }

    NODISCARD
    constexpr
    auto
    operator!=(
        const void* const rhs
    ) const noexcept -> bool
    {
        return !(*this == rhs);
    }

    NODISCARD
    constexpr
    explicit
    operator
    bool() const noexcept
    {
        return valid();
    }

    NODISCARD
    constexpr
    auto
    operator*() const noexcept -> const void*
    {
        return get();
    }

    NODISCARD
    constexpr
    auto
    get() const noexcept -> const void*
    {
        return handle_;
    }

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return CloseGuard::valid(handle_);
    }

    ZEN_CXX23_CONSTEXPR
    auto
    release() noexcept -> const void*
    {
        const auto* const old = get();

        handle_ = CloseGuard::invalid_value();

        return old;
    }

    ZEN_CXX23_CONSTEXPR
    auto
    reset(
        const void* const value = CloseGuard::invalid_value()
    ) noexcept -> handle_guard&
    {
        if (valid()) {
            CloseGuard::close(get());
        }

        handle_ = value;

        return *this;

    }

private:
    const void* handle_{CloseGuard::invalid_value()};
};

using native_handle = handle_guard<detail::nt_close_guard>;
} //namespace win
} //namespace zen
