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

#include <zen/coff/string.hpp>

ZEN_COFF_ALIGNMENT(zen)
namespace coff {
enum struct section_flags : u32
{
    type_reg                          = 0x00000000,
    type_d_sect                       = 0x00000001,
    type_no_load                      = 0x00000002,
    type_group                        = 0x00000004,
    type_no_padded                    = 0x00000008,
    type_copy                         = 0x00000010,
    content_code                      = 0x00000020,
    content_initialized_data          = 0x00000040,
    content_uninitialized_data        = 0x00000080,
    link_other                        = 0x00000100,
    link_info                         = 0x00000200,
    type_over                         = 0x00000400,
    link_remove                       = 0x00000800,
    link_com_dat                      = 0x00001000,
    no_defer_spec_exceptions          = 0x00004000,
    relative_gp                       = 0x00008000,
    mem_purgeable                     = 0x00020000,
    memory_16_bit                     = 0x00020000,
    memory_locked                     = 0x00040000,
    memory_preload                    = 0x00080000,
    align_1_bytes                     = 0x00100000,
    align_2_bytes                     = 0x00200000,
    align_4_bytes                     = 0x00300000,
    align_8_bytes                     = 0x00400000,
    align_16_bytes                    = 0x00500000,
    align_32_bytes                    = 0x00600000,
    align_64_bytes                    = 0x00700000,
    align_128_bytes                   = 0x00800000,
    align_256_bytes                   = 0x00900000,
    align_512_bytes                   = 0x00A00000,
    align_1024_bytes                  = 0x00B00000,
    align_2048_bytes                  = 0x00C00000,
    align_4096_bytes                  = 0x00D00000,
    align_8192_bytes                  = 0x00E00000,
    link_extended_relocation_overflow = 0x01000000,
    memory_discardable                = 0x02000000,
    memory_not_cached                 = 0x04000000,
    memory_not_paged                  = 0x08000000,
    memory_shared                     = 0x10000000,
    memory_execute                    = 0x20000000,
    memory_read                       = 0x40000000,
    memory_write                      = 0x80000000
};
ZEN_ENUM_OPERATORS(section_flags);

union section_characteristics
{
    u32 flags;
    struct
    {
        u32 _pad0             : 5;
        u32 cnt_code          : 1; // Section contains code.
        u32 cnt_init_data     : 1; // Section contains initialized data.
        u32 cnt_uninit_data   : 1; // Section contains uninitialized data.
        u32 _pad1             : 1;
        u32 lnk_info          : 1; // Section contains comments or some other type of information.
        u32 _pad2             : 1;
        u32 lnk_remove        : 1; // Section contents will not become part of image.
        u32 lnk_comdat        : 1; // Section contents comdat.
        u32 _pad3             : 1;
        u32 no_defer_spec_exc : 1; // Reset speculative exceptions handling bits in the TLB entries for this section.
        u32 mem_far           : 1;
        u32 _pad4             : 1;
        u32 mem_purgeable     : 1;
        u32 mem_locked        : 1;
        u32 mem_preload       : 1;
        u32 alignment         : 4; // Alignment calculated as: n ? 1 << (n - 1) : 16
        u32 lnk_nreloc_ovfl   : 1; // Section contains extended relocations.
        u32 mem_discardable   : 1; // Section can be discarded.
        u32 mem_not_cached    : 1; // Section is not cachable.
        u32 mem_not_paged     : 1; // Section is not pageable.
        u32 mem_shared        : 1; // Section is shareable.
        u32 mem_execute       : 1; // Section is executable.
        u32 mem_read          : 1; // Section is readable.
        u32 mem_write         : 1; // Section is writeable.
    };
};

class section_header
{
    struct native
    {
        scn_string name{};
        union
        {
            u32 physical_address;
            u32 virtual_size;
        };
        u32                     virtual_address{};
        u32                     size_raw_data{};
        u32                     ptr_raw_data{};
        u32                     ptr_relocs{};
        u32                     ptr_line_numbers{};
        u16                     num_relocs{};
        u16                     num_line_numbers{};
        section_characteristics characteristics{};
    };

public:
    constexpr
    section_header() noexcept = default;

    NODISCARD
    constexpr
    auto
    name() noexcept -> scn_string&
    {
        return ctx_.name;
    }

    NODISCARD
    constexpr
    auto
    name() const noexcept -> const scn_string&
    {
        return const_cast<section_header*>(this)->name();
    }

    NODISCARD
    constexpr
    auto
    physical_address() const noexcept -> u32
    {
        return bit::little(ctx_.physical_address);
    }

    constexpr
    auto
    physical_address(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.physical_address = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    virtual_size() const noexcept -> u32
    {
        return bit::little(ctx_.virtual_size);
    }

    constexpr
    auto
    virtual_size(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.virtual_size = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    virtual_address() const noexcept -> u32
    {
        return bit::little(ctx_.virtual_address);
    }

    constexpr
    auto
    virtual_address(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.virtual_address = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_raw_data() const noexcept -> u32
    {
        return bit::little(ctx_.size_raw_data);
    }

    constexpr
    auto
    size_raw_data(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.size_raw_data = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ptr_raw_data() const noexcept -> u32
    {
        return bit::little(ctx_.ptr_raw_data);
    }

    constexpr
    auto
    ptr_raw_data(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.ptr_raw_data = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ptr_relocs() const noexcept -> u32
    {
        return bit::little(ctx_.ptr_relocs);
    }

    constexpr
    auto
    ptr_relocs(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.ptr_relocs = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ptr_line_numbers() const noexcept -> u32
    {
        return bit::little(ctx_.ptr_line_numbers);
    }

    constexpr
    auto
    ptr_line_numbers(
        const u32 value
    ) noexcept -> section_header&
    {
        ctx_.ptr_line_numbers = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_relocs() const noexcept -> u16
    {
        return bit::little(ctx_.num_relocs);
    }

    constexpr
    auto
    num_relocs(
        const u16 value
    ) noexcept -> section_header&
    {
        ctx_.num_relocs = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_line_numbers() const noexcept -> u16
    {
        return bit::little(ctx_.num_line_numbers);
    }

    constexpr
    auto
    num_line_numbers(
        const u16 value
    ) noexcept -> section_header&
    {
        ctx_.num_line_numbers = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    characteristics() const noexcept -> section_characteristics
    {
        return {.flags = bit::little(ctx_.characteristics.flags)};
    }

    template<class T>
    requires(std::is_same_v<u32, T> || std::is_same_v<section_characteristics, T>)
    constexpr
    auto
    characteristics(
        const T& value
    ) noexcept -> section_header&
    {
        if constexpr (std::is_same_v<section_characteristics, T>) {
            ctx_.characteristics.flags = bit::little(value.flags);
        } else {
            ctx_.characteristics.flags = bit::little(value);
        }

        return *this;
    }
    NODISCARD
    auto
    paged() const noexcept -> bool
    {
        return !characteristics().mem_not_paged
            && (
                bit::little(
                    *reinterpret_cast<const u32*>(&ctx_.name.short_name)
                ) == 0x6164652E /*'ade.'*/
            || bit::little(
                *reinterpret_cast<const u32*>(&ctx_.name.short_name)
                ) == 0x45474150 /*'EGAP'*/
        );
    }

    NODISCARD
    auto
    discardable() const noexcept -> bool
    {
        return characteristics().mem_discardable
            || bit::little(
                *reinterpret_cast<const u32*>(&ctx_.name.short_name)
            ) == 0x54494E49 /*'TINI'*/;
    }

    NODISCARD
    constexpr
    auto
    inside(
        const u32 rva
    ) const noexcept -> bool
    {
        return virtual_address() <= rva
            && rva < (virtual_address() + virtual_size());
    }

private:
    native ctx_{};
};
} //namespace coff

namespace win {
using section_flags           = coff::section_flags;
using section_characteristics = coff::section_characteristics;
using section_header          = coff::section_header;
} //namespace win
ZEN_RESTORE_ALIGNMENT() //namespace zen

