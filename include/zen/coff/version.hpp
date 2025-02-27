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

#include <zen/core/bit.hpp>

ZEN_COFF_ALIGNMENT(zen)
namespace coff {
template<std::integral T>
requires(sizeof(T) >= sizeof(u16) && std::is_unsigned_v<T>)
class version
{
public:
    using value_type = T;
    using half_type = std::conditional_t<
        sizeof(T) == sizeof(u64),
        u32,
        std::conditional_t<
            sizeof(T) == sizeof(u32),
            u16,
            u8
        >
    >;

    union native
    {
        value_type identifier{};

        struct
        {
            half_type major;
            half_type minor;
        };
    };

    constexpr
    version() noexcept = default;

    constexpr
    explicit
    version(
        const value_type identifier
    ) noexcept
        : ctx_{.identifier = identifier}
    {}

    constexpr
    version(
        const half_type major,
        const half_type minor
    ) noexcept
    {
        ctx_.major = major;
        ctx_.minor = minor;
    }

    NODISCARD
    constexpr
    auto
    identifier() const noexcept -> value_type
    {
        return bit::little(ctx_.identifier);
    }

    constexpr
    auto
    identifier(
        const value_type val
    ) noexcept -> void
    {
        ctx_.identifier = bit::little(val);
    }

    NODISCARD
    constexpr
    auto
    major() const noexcept -> half_type
    {
        return bit::little(ctx_.major);
    }

    constexpr
    auto
    major(
        const half_type val
    ) noexcept -> void
    {
        ctx_.major = bit::little(val);
    }

    NODISCARD
    constexpr
    auto
    minor() const noexcept -> half_type
    {
        return bit::little(ctx_.minor);
    }

    constexpr
    auto
    minor(
        const half_type val
    ) noexcept -> void
    {
        ctx_.minor = bit::little(val);
    }

private:
    native ctx_{};
};

using version64 = version<u64>;
using version32 = version<u32>;
using version16 = version<u16>;
} //namespace coff
namespace win {
using version64 = coff::version64;
using version32 = coff::version32;
using version16 = coff::version16;
} //namespace win
ZEN_RESTORE_ALIGNMENT() //namespace zen
