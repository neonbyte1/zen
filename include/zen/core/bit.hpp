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

namespace zen::bit {
template<scalar T>
NODISCARD
constexpr
auto
swap(
    const T value
) noexcept -> T
{
    if constexpr (
        std::is_enum_v<T>
#if ZEN_HAS_CXX23
        || std::is_scoped_enum_v<T>
#endif
    ) {
        return static_cast<T>(bit::swap(std::to_underlying(value)));
    } else if constexpr (std::is_floating_point_v<T>) {
        using data_type = std::conditional_t<sizeof(T) == sizeof(f64), u64, u32>;

        return std::bit_cast<T>(bit::swap(std::bit_cast<data_type>(value)));
    } else {
        if constexpr (sizeof(T) == sizeof(u64)) {
            return static_cast<T>(
                   (value << 56)
                | ((value & 0x000000000000FF00) << 40)
                | ((value & 0x0000000000FF0000) << 24)
                | ((value & 0x000000FF00000000) >> 8)
                | ((value & 0x0000FF0000000000) >> 24)
                | ((value & 0x00FF000000000000) >> 40)
                |  (value >> 56)
            );
        } else if constexpr (sizeof(T) == sizeof(u32)) {
            return static_cast<T>(
                  ((value >> 24) & 0x000000FF)
                | ((value <<  8) & 0x00FF0000)
                | ((value >>  8) & 0x0000FF00)
                | ((value << 24) & 0xFF000000)
            );
        } else if constexpr (sizeof(T) == sizeof(u16)) {
            return static_cast<T>(value >> 8 | value << 8);
        } else {
            return value;
        }
    }
}

template<std::endian Target, scalar T>
NODISCARD
constexpr
auto
swap_if(
    const T value
) noexcept -> T
{
    if constexpr (std::endian::native == Target) {
        return bit::swap(value);
    }  else {
        return value;
    }
}

template<scalar T>
NODISCARD
constexpr
auto
little(
    const T value
) noexcept -> T
{
    return swap_if<std::endian::big>(value);
}

template<scalar T>
NODISCARD
constexpr
auto
big(
    const T value
) noexcept -> T
{
    return swap_if<std::endian::little>(value);
}
} //namespace zen::bit
