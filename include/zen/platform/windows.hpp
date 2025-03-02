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

#include <zen/core/xors.hpp>
#include <functional>

namespace zen::win {
NODISCARD
auto
get_peb() noexcept -> uptr;

NODISCARD
auto
get_pid() noexcept -> u32;

auto
enum_modules(
    const std::function<bool(std::wstring_view, va_t<>)>& callback
) noexcept -> void;

NODISCARD
auto
get_module_handle(
    u32  name,
    bool lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_module_handle(
    std::string_view name,
    bool             lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_module_handle(
    std::wstring_view name,
    bool              lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_proc_address(
    uptr             handle,
    std::string_view name,
    bool             lowercase = true
) noexcept -> uptr;

NODISCARD
auto
to_wide(
    std::string_view input
) noexcept -> std::wstring;

NODISCARD
auto
to_ansi(
    std::wstring_view input
) noexcept -> std::string;

NODISCARD
auto
get_proc_address(
    uptr handle,
    u32  name,
    bool lowercase = true
) noexcept -> uptr;

template<class T>
requires(sizeof(T) == sizeof(uptr))
NODISCARD
auto
get_proc_address(
    const uptr             handle,
    const std::string_view name,
    const bool             lowercase = true
) noexcept -> T
{
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(get_proc_address(handle, name, lowercase));
    } else {
        return reinterpret_cast<T>(get_proc_address(handle, name, lowercase));
    }
}

template<class T>
requires(sizeof(T) == sizeof(uptr))
NODISCARD
auto
get_proc_address(
    const uptr handle,
    const u32  name,
    const bool lowercase = true
) noexcept -> T
{
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(get_proc_address(handle, name, lowercase));
    } else {
        return reinterpret_cast<T>(get_proc_address(handle, name, lowercase));
    }
}
} //namespace zen::win
