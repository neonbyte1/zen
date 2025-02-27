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

ZEN_WIN32_ALIGNMENT(zen::win)
class tls_characteristics
{
    union native
    {
        u32 flags;
        struct
        {
            u32 _reserved  : 20;
            u32 alignment  : 4;
            u32 _reserved1 : 8;
        };
    };

public:
    constexpr
    tls_characteristics() noexcept = default;

    explicit
    constexpr
    tls_characteristics(
        const u32 flags
    ) noexcept
        : ctx_{.flags = flags}
    {}

    NODISCARD
    constexpr
    auto
    flags() const noexcept -> u32
    {
        return bit::little(ctx_.flags);
    }

    constexpr
    auto
    flags(
        const u32 val
    ) noexcept -> tls_characteristics&
    {
        ctx_.flags = bit::little(val);

        return *this;
    }

    template<std::integral T = szt>
    NODISCARD
    constexpr
    auto
    alignment() const noexcept -> T
    {
        return convert_alignment<T>(native{.flags = flags()}.alignment);
    }

    template<std::integral T>
    constexpr
    auto
    alignment(
        const T val
    ) noexcept -> bool
    {
        native temp{.flags = flags()};

        temp.alignment = reflect_alignment(val);

        flags(temp.flags);

        return temp.alignment > 0;
    }

private:
    native ctx_{};
};

template<bool X64 = detail::is_64_bit>
class tls_directory
{
    using address_type = va_t<X64>;

    struct native
    {
        address_type        address_raw_data_start{};
        address_type        address_raw_data_end{};
        address_type        address_index{};
        address_type        address_callbacks{};
        u32                 size_zero_fill{};
        tls_characteristics characteristics{};
    };

public:
    tls_directory() noexcept = default;

    NODISCARD
    constexpr
    auto
    address_raw_data_start() const noexcept -> address_type
    {
        return bit::little(ctx_.address_raw_data_start);
    }

    NODISCARD
    constexpr
    auto
    address_raw_data_start(
        const address_type val
    ) noexcept -> tls_directory&
    {
        ctx_.address_raw_data_start =  bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    address_raw_data_end() const noexcept -> address_type
    {
        return bit::little(ctx_.address_raw_data_end);
    }

    NODISCARD
    constexpr
    auto
    address_raw_data_end(
        const address_type val
    ) noexcept -> tls_directory&
    {
        ctx_.address_raw_data_end =  bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    address_index() const noexcept -> address_type
    {
        return bit::little(ctx_.address_index);
    }

    NODISCARD
    constexpr
    auto
    address_index(
        const address_type val
    ) noexcept -> tls_directory&
    {
        ctx_.address_index =  bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    address_callbacks() const noexcept -> address_type
    {
        return bit::little(ctx_.address_callbacks);
    }

    NODISCARD
    constexpr
    auto
    address_callbacks(
        const address_type val
    ) noexcept -> tls_directory&
    {
        ctx_.address_callbacks =  bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_zero_fill() const noexcept -> u32
    {
        return bit::little(ctx_.size_zero_fill);
    }

    NODISCARD
    constexpr
    auto
    size_zero_fill(
        const u32 val
    ) noexcept -> tls_directory&
    {
        ctx_.size_zero_fill =  bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    characteristics() noexcept -> tls_characteristics&
    {
        return ctx_.characteristics;
    }

    NODISCARD
    constexpr
    auto
    characteristics() const noexcept -> const tls_characteristics&
    {
        return const_cast<tls_directory*>(this)->characteristics();
    }

private:
    native ctx_{};
};

template<bool X64 = detail::is_64_bit>
using tls_callback_fn = void(ZEN_WIN32_CC*)(va_t<X64>, u32, va_t<X64>);
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
