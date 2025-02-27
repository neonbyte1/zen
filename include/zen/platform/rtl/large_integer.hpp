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
union large_integer
{
    i64 quad_part;

    struct
    {
        u32 lo;
        i32 hi;
    } u;

    constexpr
    large_integer() noexcept
        : quad_part{0}
    {}

    explicit
    constexpr
    large_integer(
        const i64 quad_part
    )
        : quad_part{quad_part}
    {}

    constexpr
    large_integer(
        const u32 lo,
        const i32 hi
    ) noexcept
        : u{.lo = lo, .hi = hi}
    {}

    constexpr
    auto
    set_ms(
        const i64 ms
    ) noexcept -> large_integer&
    {
        quad_part = ms * -10000;

        return *this;
    }

    NODISCARD
    constexpr
    auto
    get_ms() const noexcept -> i64
    {
        return quad_part / -10000;
    }
};
static_assert(sizeof(large_integer) == sizeof(i64));
} //namespace zen::rtl
