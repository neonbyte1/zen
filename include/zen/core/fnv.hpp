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
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND
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
template<class T, T PrimeValue, T OffsetValue>
requires(std::is_same_v<u32, T> || std::is_same_v<u64, T>)
class fnv_1_a
{
public:
    using value_type = T;

private:
    template<class C, bool Lowercase>
    requires(std::is_same_v<char, C> || std::is_same_v<wchar_t, C>)
    NODISCARD
    constexpr
    static
    auto
    character(const C c) noexcept -> value_type
    {

        if constexpr (Lowercase) {
            return c >= 'A' && c <= 'Z'
                ? static_cast<value_type>(c + ('a' - 'A'))
                : static_cast<value_type>(c);

        } else {
            return static_cast<value_type>(c);
        }
    }

    constexpr
    fnv_1_a() noexcept = default;

    template<bool Lowercase = false, class C>
    requires(std::is_same_v<char, C> || std::is_same_v<wchar_t, C>)
    NODISCARD
    constexpr
    static
    ZEN_FORCEINLINE
    auto
    process(
        const C* const   str,
        szt              length,
        const value_type offset = OffsetValue
    ) noexcept -> value_type
    {
        return str[0] == '\0' || length == 0
            ? offset
            : process<Lowercase>(
                &str[1],
                --length,
                (offset ^ character<C, Lowercase>(str[0])) * PrimeValue
            );
    }

public:
    NODISCARD
    constexpr
    static
    auto
    valid(
        const value_type hash_value
    ) noexcept -> bool
    {
        return hash_value != 0 && hash_value != OffsetValue;
    }

    template<bool Lowercase = false, class C>
    requires(std::is_same_v<char, C> || std::is_same_v<wchar_t, C>)
    NODISCARD
    constexpr
    static
    ZEN_FORCEINLINE
    auto
    hash(
        const C* const   str,
        const value_type offset = OffsetValue
    ) noexcept -> value_type
    {
        return process<Lowercase>(str, SIZE_MAX, offset);
    }

    template<bool Lowercase = false>
    NODISCARD
    constexpr
    static
    ZEN_FORCEINLINE
    auto
    get(
        const std::string_view str
    ) noexcept -> value_type
    {
        return !str.empty() ? process<Lowercase>(str.data(), str.length()) : 0;
    }

    template<bool Lowercase = false>
    NODISCARD
    constexpr
    static
    ZEN_FORCEINLINE
    auto
    get(
        const std::wstring_view str
    ) noexcept -> value_type
    {
        return !str.empty() ? process<Lowercase>(str.data(), str.length()) : 0;
    }
};
} //namespace detail

template<class T = u32>
requires(std::is_same_v<u32, T> || std::is_same_v<u64, T>)
using fnv = std::conditional_t<
    std::is_same_v<u32, T>,
    detail::fnv_1_a<u32, 0x1000193, 0x811C9DC5>,
    detail::fnv_1_a<u64, 0x00000100000001B3, 0xCBF29CE484222325>
>;
} //namespace zen
