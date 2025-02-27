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

#include <zen/nt/nt_headers.hpp>

ZEN_WIN32_ALIGNMENT(zen::win)
class dos_header
{
    struct native
    {
        u16    e_magic{};
        u16    e_cblp{};
        u16    e_cp{};
        u16    e_crlc{};
        u16    e_cparhdr{};
        u16    e_minalloc{};
        u16    e_maxalloc{};
        u16    e_ss{};
        u16    e_sp{};
        u16    e_csum{};
        u16    e_ip{};
        u16    e_cs{};
        u16    e_lfarlc{};
        u16    e_ovno{};
        u16    e_res[4]{};
        u16    e_oemid{};
        u16    e_oeminfo{};
        u16    e_res2[10]{};
        long_t e_lfanew{};
    };

public:
    constexpr static u16 magic_value = 0x5A4D; // "MZ"

    constexpr
    dos_header() noexcept = default;

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return magic() == magic_value;
    }

    NODISCARD
    constexpr
    auto
    magic() const noexcept -> u16
    {
        return bit::little(ctx_.e_magic);
    }

    constexpr
    auto
    magic(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_magic = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_bytes_last_page() const noexcept -> u16
    {
        return bit::little(ctx_.e_cblp);
    }

    constexpr
    auto
    num_bytes_last_page(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_cblp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_pages() const noexcept -> u16
    {
        return bit::little(ctx_.e_cp);
    }

    constexpr
    auto
    num_pages(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_cp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_relocs() const noexcept -> u16
    {
        return bit::little(ctx_.e_crlc);
    }

    constexpr
    auto
    num_relocs(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_crlc = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_of_header_in_paragraphs() const noexcept -> u16
    {
        return bit::little(ctx_.e_cparhdr);
    }

    constexpr
    auto
    size_of_header_in_paragraphs(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_cparhdr = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_min_extra_paragraphs() const noexcept -> u16
    {
        return bit::little(ctx_.e_minalloc);
    }

    constexpr
    auto
    num_min_extra_paragraphs(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_minalloc = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_max_extra_paragraphs() const noexcept -> u16
    {
        return bit::little(ctx_.e_maxalloc);
    }

    constexpr
    auto
    num_max_extra_paragraphs(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_maxalloc = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ss() const noexcept -> u16
    {
        return bit::little(ctx_.e_ss);
    }

    constexpr
    auto
    ss(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_ss = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    checksum() const noexcept -> u16
    {
        return bit::little(ctx_.e_csum);
    }

    constexpr
    auto
        checksum(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_csum = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ip() const noexcept -> u16
    {
        return bit::little(ctx_.e_ip);
    }

    constexpr
    auto
    ip(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_ip = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    cs() const noexcept -> u16
    {
        return bit::little(ctx_.e_cs);
    }

    constexpr
    auto
    cs(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_cs = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    reloc_table_offset() const noexcept -> u16
    {
        return bit::little(ctx_.e_lfarlc);
    }

    constexpr
    auto
    reloc_table_offset(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_lfarlc = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    overlay_number() const noexcept -> u16
    {
        return bit::little(ctx_.e_ovno);
    }

    constexpr
    auto
    overlay_number(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_ovno = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    oem_id() const noexcept -> u16
    {
        return bit::little(ctx_.e_oemid);
    }

    constexpr
    auto
    oem_id(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_oemid = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    oem_info() const noexcept -> u16
    {
        return bit::little(ctx_.e_oeminfo);
    }

    constexpr
    auto
    oem_info(
        const u16 val
    ) noexcept -> dos_header&
    {
        ctx_.e_oeminfo = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    next_hdr_offset() const noexcept -> long_t
    {
        return bit::little(ctx_.e_lfanew);
    }

    constexpr
    auto
    next_hdr_offset(
        const long_t val
    ) noexcept -> dos_header&
    {
        ctx_.e_lfanew = bit::little(val);

        return *this;
    }

    template<bool X64 = detail::is_64_bit>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    nt_hdr() noexcept -> nt_headers<X64>*
    {
        return reinterpret_cast<nt_headers<X64>*>(
            reinterpret_cast<std::byte*>(this) + next_hdr_offset()
        );
    }

    template<bool X64 = detail::is_64_bit>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    nt_hdr() const noexcept -> const nt_headers<X64>*
    {
        return const_cast<dos_header*>(this)->nt_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    is_32_bit() const noexcept -> bool
    {
        return nt_hdr()->is_32_bit();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    is_64_bit() const noexcept -> bool
    {
        return nt_hdr()->is_64_bit();
    }

    NODISCARD
    auto
    file_hdr() noexcept -> file_header*
    {
        return &nt_hdr()->file_hdr();
    }

    NODISCARD
    auto
    file_hdr() const noexcept -> const file_header*
    {
        return const_cast<dos_header*>(this)->file_hdr();
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
