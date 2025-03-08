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
class memory_guard
{
public:
    memory_guard(
        const void* const     handle,
        const u64             size,
        const page_protection protection = page_protection::execute_readwrite,
        const allocation_type allocation = allocation_type::commit | allocation_type::reserve
    ) noexcept
        : handle_{handle}
        , size_{size}
        , protection_{protection}
        , allocation_{allocation}
    {
        nt_allocate_virtual_memory(handle, &address_, 0, size, allocation, protection);
    }

    constexpr
    memory_guard(
        const memory_guard& rhs
    ) = delete;

    constexpr
    memory_guard(
        memory_guard&& rhs
    ) noexcept
        : handle_{rhs.handle_}
        , address_{rhs.release()}
        , size_{rhs.size_}
        , protection_{rhs.protection_}
        , allocation_{rhs.allocation_}
    {
        rhs.handle_     = nullptr;
        rhs.size_       = 0;
        rhs.protection_ = page_protection{};
        rhs.allocation_ = allocation_type{};
    }

    ~memory_guard() noexcept
    {
        reset();
    }

    auto
    operator=(
        const memory_guard& rhs
    ) -> memory_guard& = delete;

    auto
    operator=(
        memory_guard&& rhs
    ) noexcept -> memory_guard&
    {
        reset(rhs.release());

        handle_     = rhs.handle_;
        size_       = rhs.size_;
        protection_ = rhs.protection_;
        allocation_ = rhs.allocation_;

        rhs.handle_     = nullptr;
        rhs.size_       = 0;
        rhs.protection_ = page_protection{};
        rhs.allocation_ = allocation_type{};

        return *this;
    }

    NODISCARD
    constexpr
    auto
    operator*() const noexcept -> u64
    {
        return get();
    }

    NODISCARD
    constexpr
    explicit
    operator
    bool() const noexcept
    {
        return valid();
    }

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return address_ != 0;
    }

    NODISCARD
    constexpr
    auto
    get() const noexcept -> u64
    {
        return address_;
    }

    NODISCARD
    constexpr
    auto
    size() const noexcept -> u64
    {
        return size_;
    }

    template<std::integral T>
    constexpr
    auto
    size() const noexcept -> T
    {
        return static_cast<T>(size());
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
    allocation() const noexcept -> allocation_type
    {
        return allocation_;
    }

    constexpr
    auto
    release() noexcept -> u64
    {
        const auto old = get();

        address_ = 0;

        return old;
    }

    auto
    reset(
        const u64 address = 0
    ) noexcept -> memory_guard&
    {
        if (valid()) {
            nt_free_virtual_memory(
                handle_,
                address_,
                0,
                std::to_underlying(allocation_type::release)
            );
        }

        address_ = address;

        return *this;
    }

private:
    const void*     handle_{};
    u64             address_{};
    u64             size_{};
    page_protection protection_{};
    allocation_type allocation_{};
};
} //namespace zen::win
