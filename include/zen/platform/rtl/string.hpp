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

namespace zen::rtl {
template<class T, class C>
requires(std::is_integral_v<T> && sizeof(T) >= sizeof(u32) && (std::is_same_v<char, C> || std::is_same_v<wchar_t, C>))
class string_t
{
public:
    using value_type       = C;
    using pointer          = C*;
    using const_pointer    = const C*;
    using string_view_type = std::basic_string_view<value_type>;
    using string_type      = std::basic_string<value_type>;

private:
    template<class Ret>
    requires(std::is_same_v<string_view_type, Ret> || std::is_same_v<string_type, Ret>)
    NODISCARD
    constexpr
    auto
    resolve() const noexcept(std::is_same_v<string_view_type, C>) -> Ret
    {
        if (buffer_ && length()) {
            return {
                reinterpret_cast<const C*>(buffer_),
                static_cast<szt>(len())
            };
        }

        return {};
    }

public:
    NODISCARD
    constexpr
    auto
    len() const noexcept -> u16
    {
        return len_;
    }

    constexpr
    auto
    len(
        const u16 val
    ) noexcept -> string_t&
    {
        len_ = val;

        return *this;
    }

    NODISCARD
    constexpr
    auto
    max_len() const noexcept -> u16
    {
        return max_len_;
    }

    constexpr
    auto
    max_len(
        const u16 val
    ) noexcept -> string_t&
    {
        max_len_ = val;

        return *this;
    }

    NODISCARD
    constexpr
    auto
    buffer() noexcept -> T&
    {
        return buffer_;
    }

    NODISCARD
    constexpr
    auto
    buffer() const noexcept -> const T&
    {
        return const_cast<string_t*>(this)->buffer();
    }

    constexpr
    auto
    buffer(
        const T val
    ) noexcept -> string_t&
    {
        buffer_ = val;

        return *this;
    }

    NODISCARD
    constexpr
    static
    auto
    max_size() noexcept -> szt
    {
        return (USHRT_MAX & -1) - sizeof(C);
    }

    NODISCARD
    constexpr
    auto
    empty() const noexcept -> bool
    {
        return len() <= sizeof(C);
    }

    NODISCARD
    constexpr
    auto
    length() const noexcept -> szt
    {
        return static_cast<szt>(len()) / sizeof(C);
    }

    NODISCARD
    constexpr
    auto
    max_length() const noexcept -> szt
    {
        return static_cast<szt>(max_len()) / sizeof(C);
    }

    NODISCARD
    constexpr
    auto
    get() const noexcept -> string_view_type
    {
        return resolve<string_view_type>();
    }

    NODISCARD
    constexpr
    auto
    generic() const -> string_type
    {
        return resolve<string_type>();
    }

    NODISCARD
    constexpr
    auto
    equals(
        const string_t& other
    ) const noexcept -> bool
    {
        return equals(other.get());
    }

    NODISCARD
    constexpr
    auto
    equals(
        const string_view_type other
    ) const noexcept -> bool
    {
        return get() == other;
    }

    auto
    assign(
        const string_view_type data
    ) noexcept -> string_t&
    {
        if (data.empty()) {
            len(0);
            max_len(0);
            buffer(value_type{});
        } else {
            len(static_cast<u16>(data.length() * sizeof(C)));
            max_len(len() + sizeof(*this));
            buffer(reinterpret_cast<const T>(data.data()));
        }

        return *this;
    }

private:
    u16 len_{};
    u16 max_len_{};
    T   buffer_{};
};

template<bool X64 = detail::is_64_bit>
using unicode_string = string_t<va_t<X64>, wchar_t>;

template<bool X64 = detail::is_64_bit>
using ansii_string = string_t<va_t<X64>, char>;
} //namespace zen::rtl
