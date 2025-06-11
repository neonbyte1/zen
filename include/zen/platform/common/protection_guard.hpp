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

#include <zen/platform/syscall.hpp>

namespace zen::win {
class protection_guard
{
    template<class T>
    NODISCARD
    static
    auto
    load_address(
        T addr
    ) noexcept -> u64
    {
        if constexpr (std::is_integral_v<T>) {
            return static_cast<u64>(addr);
        } else {
            return reinterpret_cast<const u64>(addr);
        }
    }
public:
    template<class T>
    requires(std::is_pointer_v<T> || std::is_integral_v<T>)
    protection_guard(
        const void* const     handle,
        T                     addr,
        const szt             size,
        const page_protection protection = page_protection::execute_readwrite
    ) noexcept
        : handle_{handle}
        , base_address_{load_address(addr)}
        , size_{size}
    {
        nt_protect_virtual_memory(
            handle_,
            base_address_,
            size_,
            protection,
            &old_protection_
        );
    }

    template<class T>
    requires(std::is_pointer_v<T> || std::is_integral_v<T>)
    protection_guard(
        T                     addr,
        const szt             size,
        const page_protection protection = page_protection::execute_readwrite
    ) noexcept
        : protection_guard{invalid_handle_value(), addr, size, protection}
    {}

    protection_guard(
        const protection_guard& rhs
    ) = delete;

    protection_guard(
        protection_guard&& rhs
    ) noexcept = delete;

    ~protection_guard()
    {
        page_protection tmp{};

        nt_protect_virtual_memory(
            handle_,
            base_address_,
            size_,
            old_protection_,
            &tmp
        );
    }

    auto
    operator=(
        const protection_guard& rhs
    ) -> protection_guard& = delete;

    auto
    operator=(
        protection_guard&& rhs
    ) -> protection_guard& = delete;

    NODISCARD
    constexpr
    auto
    old_protection() const noexcept -> page_protection
    {
        return old_protection_;
    }

private:
    const void*     handle_;
    u64             base_address_{};
    u64             size_{};
    page_protection old_protection_{};
};
} //namespace zen::win
