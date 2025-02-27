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

#include <zen/nt/dos_header.hpp>

ZEN_WIN32_ALIGNMENT(zen::win)
template<bool X64 = detail::is_64_bit>
class image
{
public:
    constexpr
    image() noexcept = default;

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    valid() const noexcept -> bool
    {
        return dos_hdr_.valid() && nt_hdr()->valid();
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
    constexpr
    auto
    dos_hdr() noexcept -> dos_header*
    {
        return &dos_hdr_;
    }

    NODISCARD
    constexpr
    auto
    dos_hdr() const noexcept -> const dos_header*
    {
        return const_cast<image*>(this)->dos_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    nt_hdr() noexcept -> nt_headers<X64>*
    {
        return dos_hdr_.nt_hdr<X64>();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    nt_hdr() const noexcept -> const nt_headers<X64>*
    {
        return const_cast<image*>(this)->nt_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    optional_hdr() noexcept -> optional_header<X64>*
    {
        return &nt_hdr()->optional_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    optional_hdr() const noexcept -> const optional_header<X64>*
    {
        return const_cast<image*>(this)->optional_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    file_hdr() noexcept -> file_header*
    {
        return &nt_hdr()->file_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    file_hdr() const noexcept -> const file_header*
    {
        return const_cast<image*>(this)->file_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    directory(
        const win::directory id
    ) noexcept -> data_directory*
    {
        auto* const opt = optional_hdr();

        if (opt->num_data_directories() <= static_cast<u16>(id)) {
            return nullptr;
        }

        auto* const dir = &opt->data_directories().at(id);

        return dir->present() ? dir : nullptr;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    directory(
        const win::directory id
    ) const noexcept -> const data_directory*
    {
        return const_cast<image*>(this)->directory(id);
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    raw_limit() const noexcept -> szt
    {
        const auto* const nt  = nt_hdr();
        const auto* const scn = nt->template sections<false>();

        szt max_raw = nt->optional_hdr().size_headers();

        for (szt i{}; i < nt->file_hdr().num_sections(); ++i) {
            max_raw = std::max<szt>(scn[i].ptr_raw_data() + scn[i].size_raw_data(), max_raw);
        }

        if (const auto* const dir = directory(win::directory::security)) {
            max_raw = std::max<szt>(dir->rva() + dir->size(), max_raw);
        }

        return max_raw;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_section(
        const u32 rva
    ) noexcept -> coff::section_header*
    {
        auto* const nt = nt_hdr();

        for (szt i{}; i < nt->file_hdr().num_sections(); ++i) {
            auto* const section = nt->section(i);

            if (
                section->virtual_address() <= rva
                && rva < (section->virtual_address() + section->virtual_size())
            ) {
                return section;
            }
        }

        return nullptr;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_section(
        const u32 rva
    ) const noexcept -> const coff::section_header*
    {
        return const_cast<image*>(this)->rva_to_section(rva);
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_section(
        const u32 offset
    ) noexcept -> coff::section_header*
    {
        auto* const nt = nt_hdr();

        for (szt i{}; i < nt->file_hdr().num_sections(); ++i) {
            auto* const section = nt->section(i);

            if (
                section->ptr_raw_data() <= offset
                && offset < (section->ptr_raw_data() + section->size_raw_data())
            ) {
                return section;
            }
        }

        return nullptr;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_section(
        const u32 offset
    ) const noexcept -> const coff::section_header*
    {
        return const_cast<image*>(this)->fo_to_section(offset);
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_ptr(
        const u32 rva,
        const szt length = 1
    ) noexcept -> T*
    {
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
        (void)length;

        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_) + rva
        );
#else
        const auto* const scn = rva_to_section(rva);

        if (!scn) {
            const auto rva_hdr_end = nt_hdr()->optional_hdr().size_headers();

            if (rva < rva_hdr_end && (rva + length) <= rva_hdr_end) {
                return reinterpret_cast<T*>(
                    reinterpret_cast<std::byte*>(&dos_hdr_) + rva
                );
            }

            return nullptr;
        }

        // Apply the boundary check.
        const szt offset = rva - scn->virtual_address();

        if ((offset + length) > scn->size_raw_data()) {
            return nullptr;
        }


        // Return the final pointer.
        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_)
            + scn->ptr_raw_data()
            + offset
        );
#endif
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_ptr(
        const u32 rva,
        const szt length = 1
    ) const noexcept -> const T*
    {
        return const_cast<image*>(this)->template rva_to_ptr<T>(rva, length);
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_ptr(
        const u32 offset,
        const szt length = 1
    ) noexcept -> T*
    {
        const auto* const scn = fo_to_section(offset);

        if (!scn) {
            const auto rva_hdr_end = nt_hdr()->optional_hdr().size_headers();

            if (offset < rva_hdr_end && (offset + length) <= rva_hdr_end) {
                return reinterpret_cast<T*>(
                    reinterpret_cast<std::byte*>(&dos_hdr_) + offset
                );
            }

            return nullptr;
        }

        // Apply the boundary check.
        const szt soffset = offset - scn->ptr_raw_data();
        if ((soffset + length) > scn->virtual_size()) {
            return nullptr;
        }

        // Return the final pointer.
        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_)
            + scn->virtual_address()
            + soffset
        );
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_ptr(
        const u32 offset,
        const szt length = 1
    ) const noexcept -> const T*
    {
        return const_cast<image*>(this)->template fo_to_ptr<T>(offset, length);
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    raw_to_ptr(
        const u32 offset,
        const szt length = 0
    ) noexcept -> T*
    {
        if (length != 0 && (offset + length) > raw_limit()) {
            return nullptr;
        }

        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_) + offset
        );
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    raw_to_ptr(
        const u32 offset,
        const szt length = 0
    ) const noexcept -> const T*
    {
        return const_cast<image*>(this)->template raw_to_ptr<T>(offset, length);
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    static
    auto
    make(
        va_t<X64> handle
    ) noexcept -> image*
    {
        return reinterpret_cast<image*>(handle);
    }

private:

    dos_header dos_hdr_;
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
