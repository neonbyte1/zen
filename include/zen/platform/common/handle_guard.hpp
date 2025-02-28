// Copyright (c) 2025, neonbyte - All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
