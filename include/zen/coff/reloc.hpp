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

ZEN_COFF_ALIGNMENT(zen::coff)
enum struct reloc_type : u16
{
    amd64_absolute = 0x0000,    // The relocation is ignored.
    amd64_addr64   = 0x0001,    // The 64-bit VA of the relocation target.
    amd64_addr32   = 0x0002,    // The 32-bit VA of the relocation target.
    amd64_addr32nb = 0x0003,    // The 32-bit address without an image base (RVA).
    amd64_rel32    = 0x0004,    // The 32-bit relative address from the byte following the relocation.
    amd64_rel32_1  = 0x0005,    // The 32-bit address relative to byte distance 1 from the relocation.
    amd64_rel32_2  = 0x0006,    // The 32-bit address relative to byte distance 2 from the relocation.
    amd64_rel32_3  = 0x0007,    // The 32-bit address relative to byte distance 3 from the relocation.
    amd64_rel32_4  = 0x0008,    // The 32-bit address relative to byte distance 4 from the relocation.
    amd64_rel32_5  = 0x0009,    // The 32-bit address relative to byte distance 5 from the relocation.
    amd64_section  = 0x000A,    // The 16-bit section index of the section that contains the target. This is used to support debugging information.
    amd64_secrel   = 0x000B,    // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information and static thread local storage.
    amd64_secrel7  = 0x000C,    // A 7-bit unsigned offset from the base of the section that contains the target.
    amd64_token    = 0x000D,    // CLR tokens.
    amd64_srel32   = 0x000E,    // A 32-bit signed span-dependent value emitted into the object.
    amd64_pair     = 0x000F,    // A pair that must immediately follow every span-dependent value.
    amd64_sspan32  = 0x0010,    // A 32-bit signed span-dependent value that is applied at link time.

    i386_absolute = 0x0000,     // The relocation is ignored.
    i386_dir16    = 0x0001,     // Not supported.
    i386_rel16    = 0x0002,     // Not supported.
    i386_dir32    = 0x0006,     // The target's 32-bit VA.
    i386_dir32nb  = 0x0007,     // The target's 32-bit RVA.
    i386_seg12    = 0x0009,     // Not supported.
    i386_section  = 0x000A,     // The 16-bit section index of the section that contains the target. This is used to support debugging information.
    i386_secrel   = 0x000B,     // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information and static thread local storage.
    i386_token    = 0x000C,     // The CLR token.
    i386_secrel7  = 0x000D,     // A 7-bit offset from the base of the section that contains the target.
    i386_rel32    = 0x0014,     // The 32-bit relative displacement to the target. This supports the x86 relative branch and call instructions.
};

class reloc
{
    struct native
    {
        u32        virtual_address{};
        u32        symbol_index{};
        reloc_type type{};
    };

public:
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
    ) noexcept -> reloc&
    {
        ctx_.virtual_address = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    symbol_index() const noexcept -> u32
    {
        return bit::little(ctx_.symbol_index);
    }

    constexpr
    auto
    symbol_index(
        const u32 value
    ) noexcept -> reloc&
    {
        ctx_.symbol_index = bit::little(value);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    type() const noexcept -> reloc_type
    {
        return bit::little(ctx_.type);
    }

    constexpr
    auto
    type(
        const reloc_type value
    ) noexcept -> reloc&
    {
        ctx_.type = bit::little(value);

        return *this;
    }
private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::coff
