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

#include <zen/core/fnv.hpp>

namespace zen::detail {
template<i32 N>
struct random_generator
{
    // 1988, Stephen Park and Keith Miller
    // "Random Number Generators: Good Ones Are Hard To Find", considered as "minimal standard"
    // Park-Miller 31 bit pseudo-random number generator, implemented with G. Carta's optimisation:
    // with 32-bit math and without division

    constexpr static unsigned a     = 16807;                           // 7^5
    constexpr static unsigned s     = random_generator<N - 1>::value;
    constexpr static unsigned lo    = a * (s & 0xFFFF);                // Multiply lower 16 bits by 16807
    constexpr static unsigned hi    = a * (s >> 16);                   // Multiply higher 16 bits by 16807
    constexpr static unsigned lo2   = lo + ((hi & 0x7FFF) << 16);      // Combine lower 15 bits of hi with lo's upper bits
    constexpr static unsigned hi2   = hi >> 15;                        // Discard lower 15 bits of hi
    constexpr static unsigned lo3   = lo2 + hi;
    constexpr static unsigned max   = 2147483647;                      // 2^31 - 1
    constexpr static unsigned value = lo3 > max ? lo3 - max : lo3;
};

template<>
struct random_generator<0>
{
    static constexpr unsigned value = fnv<>::hash(__TIME__);
};

template<i32 Lo, i32 Hi>
struct random_int
{
    static constexpr auto value = random_generator<Lo + 1>::value % Hi;
};

template<class T, i32 N>
    requires(std::is_same_v<T, char> || std::is_same_v<T, wchar_t>)
struct random_char
{};

template<i32 N>
struct random_char<char, N>
{
    constexpr static auto value = static_cast<char>(1 + random_int<N, 0x7F - 1>::value);
};

template<i32 N>
struct random_char<wchar_t, N>
{
    constexpr static auto value = static_cast<wchar_t>(1 + random_int<N, 0x7FFF - 1>::value);
};

template<class T, szt N, i32 K>
    requires(std::is_same_v<char, T> || std::is_same_v<wchar_t, T>)
class xor_string
{
public:
    using value_type = T;
    using const_pointer = const T*;
    using array_type = std::array<value_type, N + 1>;

private:
    NODISCARD
    constexpr
    static
    auto
    crypt(
        const value_type c
    ) noexcept -> char
    {
        return c ^ random_char<T, K>::value;
    }

public:
    template<szt... Is>
    constexpr
    ZEN_FORCEINLINE
    xor_string(
        const_pointer const str,
        std::index_sequence<Is...>
    ) noexcept
        : buffer_{ crypt(str[Is])... }
    {}

    NODISCARD
    auto
    ZEN_FORCEINLINE
    get() noexcept -> const_pointer
    {
        for (szt i{}; i < N; ++i) {
            buffer_[i] = crypt(buffer_[i]);
        }

        buffer_[N] = '\0';

        return buffer_.data();
    }

private:
    array_type buffer_;
};

template<class T, szt N>
NODISCARD
constexpr
decltype(auto)
make_xor_string(
    const T(&str)[N]
) noexcept
{
    return xor_string<
        T,
        N - 1,
        __COUNTER__
    >(str, std::make_index_sequence<N - 1>());
}
} //namespace zen::detail

#if !defined(xors)
#   if defined(_NDEBUG) || defined(NDEBUG) || defined(NODEBUG)
#       define xors(s) (zen::detail::make_xor_string(s)).get()
#   else
#       define xors(s) (s)
#   endif
#endif //xors
