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

#include <zen/platform/common/handle_guard.hpp>
#include <zen/platform/constants.hpp>

namespace zen::win {
class memory_section
{
public:
    memory_section(
        page_protection prot,
        u64             size
    ) noexcept;

    NODISCARD
    constexpr
    explicit
    operator
    bool() const noexcept
    {
        return valid();
    }

    NODISCARD
    auto
    map(
        const void*     process_handle,
        allocation_type allocation = allocation_type{}
    ) const noexcept -> u64;

    NODISCARD
    constexpr
    auto
    handle() noexcept -> native_handle&
    {
        return handle_;
    }

    NODISCARD
    constexpr
    auto
    handle() const noexcept -> const native_handle&
    {
        return const_cast<memory_section*>(this)->handle();
    }

    NODISCARD
    constexpr
    auto
    protection() const noexcept -> page_protection
    {
        return protection_;
    }

    NODISCARD
    constexpr
    auto
    size() const noexcept -> u64
    {
        return size_;
    }

    template<std::integral T>
    NODISCARD
    constexpr
    auto
    size() const noexcept -> T
    {
        return static_cast<T>(size());
    }

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return handle_.valid();
    }

private:
    native_handle   handle_{};
    page_protection protection_{};
    u64             size_{};
};
} //namespace zen::win
