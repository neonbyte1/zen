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

namespace zen::win {
enum struct mouse_events : u32
{
    none             = 0,
    move             = 1 << 0,
    left_down        = 1 << 1,
    left_up          = 1 << 2,
    right_down       = 1 << 3,
    right_up         = 1 << 4,
    middle_down      = 1 << 5,
    middle_up        = 1 << 6,
    x_down           = 1 << 7,
    x_up             = 1 << 8,
    wheel            = 1 << 12,
    move_no_coalesce = 1 << 13,
    virtual_desk     = 1 << 14,
    absolute         = 1 << 15,
};
ZEN_ENUM_OPERATORS(mouse_events);

enum struct input_type : u32
{
    mouse = 0,
    keyboard,
    hardware,
};

template<bool X64 = zen::detail::is_64_bit>
struct mouse_input
{
    i32       dx{};
    i32       dy{};
    u32       mouse_data{};
    u32       flags{};
    u32       time{};
    va_t<X64> extra_info{};

    constexpr
    auto
    add_events(
        const mouse_events events
    ) noexcept -> mouse_input&
    {
        flags |= std::to_underlying(events);

        return *this;
    }

    constexpr
    auto
    set_events(
        const mouse_events events
    ) noexcept -> mouse_input&
    {
        flags = std::to_underlying(events);

        return *this;
    }
};

template<bool X64 = detail::is_64_bit>
struct keyboard_info
{
    u16       vk{};
    u16       scan{};
    u32       flags{};
    u32       time{};
    va_t<X64> extra_info{};
};

struct hardware_input
{
    u32 msg{};
    b4  param{};
};

template<bool X64 = detail::is_64_bit>
struct input
{
    input_type type{};

    union
    {
        mouse_input<X64>   mi;
        keyboard_info<X64> ki;
        hardware_input     hi;
    };

    input() noexcept
    {
        std::memset(this, 0, sizeof(*this));
    }

    explicit
    input(
        const input_type type
    ) noexcept
    {
        std::memset(this, 0, sizeof(*this));

        this->type = type;
    }
};
} //namespace zen::win
