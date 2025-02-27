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

ZEN_COFF_ALIGNMENT(zen)
namespace coff {
enum struct machine_id : u16
{
    unknown     = 0x0000,
    target_host = 0x0001,  // Useful for indicating we want to interact with the host and not a WoW guest.
    i386        = 0x014C,  // Intel 386.
    r3000       = 0x0162,  // MIPS little-endian, 0x160 big-endian
    r4000       = 0x0166,  // MIPS little-endian
    r10000      = 0x0168,  // MIPS little-endian
    wcemipsv2   = 0x0169,  // MIPS little-endian WCE v2
    alpha       = 0x0184,  // Alpha_AXP
    sh3         = 0x01A2,  // SH3 little-endian
    sh3dsp      = 0x01A3,
    sh3e        = 0x01A4,  // SH3E little-endian
    sh4         = 0x01A6,  // SH4 little-endian
    sh5         = 0x01A8,  // SH5
    arm         = 0x01C0,  // ARM Little-Endian
    thumb       = 0x01C2,  // ARM Thumb/Thumb-2 Little-Endian
    armnt       = 0x01C4,  // ARM Thumb-2 Little-Endian
    am33        = 0x01D3,
    powerpc     = 0x01F0,  // IBM PowerPC Little-Endian
    powerpcfp   = 0x01F1,
    ia64        = 0x0200,  // Intel 64
    mips16      = 0x0266,  // MIPS
    alpha64     = 0x0284,  // ALPHA64
    mipsfpu     = 0x0366,  // MIPS
    mipsfpu16   = 0x0466,  // MIPS
    axp64       = 0x0284,
    tricore     = 0x0520,  // Infineon
    cef         = 0x0CEF,
    ebc         = 0x0EBC,  // EFI Byte Code
    amd64       = 0x8664,  // AMD64 (K8)
    m32r        = 0x9041,  // M32R little-endian
    arm64       = 0xAA64,  // ARM64 Little-Endian
    cee         = 0xC0EE,
};
union file_characteristics
{
    u16     flags;
    struct
    {
        u16 relocs_stripped        : 1;  // Relocation info stripped from file.
        u16 executable             : 1;  // File is executable  (i.e. no unresolved external references).
        u16 lines_stripped         : 1;  // Line nunbers stripped from file.
        u16 local_symbols_stripped : 1;  // Local symbols stripped from file.
        u16 aggressive_ws_trim     : 1;  // Aggressively trim working set
        u16 large_address_aware    : 1;  // App can handle >2gb addresses
        u16 _pad0                  : 1;
        u16 bytes_reversed_lo      : 1;  // Bytes of machine word are reversed.
        u16 machine_32             : 1;  // 32 bit word machine.
        u16 debug_stripped         : 1;  // Debugging info stripped from file in .DBG file
        u16 runnable_from_swap     : 1;  // If Image is on removable media, copy and run from the swap file.
        u16 net_run_from_swap      : 1;  // If Image is on Net, copy and run from the swap file.
        u16 system_file            : 1;  // System File.
        u16 dll_file               : 1;  // File is a DLL.
        u16 up_system_only         : 1;  // File should only be run on a UP machine
        u16 bytes_reversed_hi      : 1;  // Bytes of machine word are reversed.
    };
};

class file_header
{
    struct native
    {
        machine_id           machine{};
        u16                  num_sections{};
        u32                  timedate_stamp{};
        u32                  ptr_symbols{};
        u32                  num_symbols{};
        u16                  size_optional_header{};
        file_characteristics characteristics{};
    };

public:
    NODISCARD
    constexpr
    auto
    machine() const noexcept -> machine_id
    {
        return bit::little(ctx_.machine);
    }

    constexpr
    auto
    machine(
        const machine_id val
    ) noexcept -> file_header&
    {
        ctx_.machine = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_sections() const noexcept -> u16
    {
        return bit::little(ctx_.num_sections);
    }

    constexpr
    auto
    num_sections(
        const u16 val
    ) noexcept -> file_header&
    {
        ctx_.num_sections = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    timedate_stamp() const noexcept -> u32
    {
        return bit::little(ctx_.timedate_stamp);
    }

    constexpr
    auto
    timedate_stamp(
        const u32 val
    ) noexcept -> file_header&
    {
        ctx_.timedate_stamp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ptr_symbols() const noexcept -> u32
    {
        return bit::little(ctx_.ptr_symbols);
    }

    constexpr
    auto
    ptr_symbols(
        const u32 val
    ) noexcept -> file_header&
    {
        ctx_.ptr_symbols = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_symbols() const noexcept -> u32
    {
        return bit::little(ctx_.num_symbols);
    }

    constexpr
    auto
    num_symbols(
        const u32 val
    ) noexcept -> file_header&
    {
        ctx_.num_symbols = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_optional_header() const noexcept -> u16
    {
        return bit::little(ctx_.size_optional_header);
    }

    constexpr
    auto
    size_optional_header(
        const u16 val
    ) noexcept -> file_header&
    {
        ctx_.size_optional_header = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    characteristics() const noexcept -> file_characteristics
    {
        return {.flags = bit::little(ctx_.characteristics.flags)};
    }

    template<class T>
    requires(std::is_same_v<file_characteristics, T> || std::is_same_v<u16, T>)
    constexpr
    auto
    characteristics(
        const T& value
    ) noexcept -> file_header&
    {
        if constexpr (std::is_same_v<file_characteristics, T>) {
            ctx_.characteristics.flags = bit::little(value.flags);
        } else {
            ctx_.characteristics.flags = bit::little(value);
        }

        return *this;
    }

private:
    native ctx_{};
};
} //namespace coff

namespace win {
using machine_id           = coff::machine_id;
using file_characteristics = coff::file_characteristics;
using file_header          = coff::file_header;
} //namespace win
ZEN_RESTORE_ALIGNMENT() //namespace zen
