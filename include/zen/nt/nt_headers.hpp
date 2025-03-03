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

#include <zen/nt/optional_header.hpp>
#include <zen/nt/iterator.hpp>
#include <zen/coff/file_header.hpp>
#include <zen/coff/section_header.hpp>

ZEN_WIN32_ALIGNMENT(zen::win)
template<bool X64 = detail::is_64_bit>
class nt_headers
{
    struct native
    {
        u32                  signature{};
        coff::file_header    file_hdr{};
        optional_header<X64> optional_hdr{};
    };

public:
    constexpr static u32 magic_value = 0x00004550; // "PE\x0\x0"

    constexpr
    nt_headers() noexcept = default;

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return signature() == magic_value;
    }

    NODISCARD
    constexpr
    auto
    signature() const noexcept -> u32
    {
        return bit::little(ctx_.signature);
    }

    constexpr
    auto
    signature(
        const u32 val
    ) noexcept -> nt_headers&
    {
        ctx_.signature = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    file_hdr() noexcept -> coff::file_header&
    {
        return ctx_.file_hdr;
    }

    NODISCARD
    constexpr
    auto
    file_hdr() const noexcept -> const coff::file_header&
    {
        return const_cast<nt_headers*>(this)->file_hdr();
    }

    NODISCARD
    constexpr
    auto
    optional_hdr() noexcept -> optional_header<X64>&
    {
        return ctx_.optional_hdr;
    }

    NODISCARD
    constexpr
    auto
    optional_hdr() const noexcept -> const optional_header<X64>&
    {
        return const_cast<nt_headers*>(this)->optional_hdr();
    }

    template<bool Iterator = false>
    NODISCARD
    auto
    sections() noexcept -> std::conditional_t<
        Iterator,
        iterator<coff::section_header>,
        coff::section_header*
    >
    {
        if constexpr (Iterator) {
            return {sections<false>(), file_hdr().num_sections()};
        } else {
            return reinterpret_cast<coff::section_header*>(
                reinterpret_cast<u8*>(&optional_hdr())
                    + file_hdr().size_optional_header()
            );
        }
    }

    template<bool Iterator = false>
    NODISCARD
    auto
    sections() const noexcept -> std::conditional_t<
        Iterator,
        iterator<const coff::section_header>,
        const coff::section_header*
    >
    {
        if constexpr (Iterator) {
            return {sections<false>(), file_hdr().num_sections()};
        } else {
            return reinterpret_cast<const coff::section_header*>(
                reinterpret_cast<const u8*>(&optional_hdr())
                    + file_hdr().size_optional_header()
            );
        }
    }

    NODISCARD
    auto
    section(
        const szt index
    ) noexcept -> coff::section_header*
    {
        return static_cast<u16>(index) >= file_hdr().num_sections()
            ? nullptr
            : sections<false>() + index;
    }

    NODISCARD
    auto
    section(
        const szt index
    ) const noexcept -> const coff::section_header*
    {
        return const_cast<nt_headers*>(this)->section(index);
    }

    NODISCARD
    constexpr
    auto
    is_32_bit() const noexcept -> bool
    {
        return optional_hdr().is_32_bit();
    }

    NODISCARD
    constexpr
    auto
    is_64_bit() const noexcept -> bool
    {
        return optional_hdr().is_64_bit();
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
