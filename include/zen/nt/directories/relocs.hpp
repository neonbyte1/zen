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
enum struct reloc_type : u16
{
    based_absolute   = 0,
    based_high       = 1,
    based_low        = 2,
    based_high_low   = 3,
    based_high_adj   = 4,
    based_ia64_imm64 = 9,
    based_dir64      = 10,
};

class reloc_entry
{
    union native
    {
        u16 value;

        struct
        {
            u16        offset : 12;
            reloc_type type   : 4;
        };
    };

    NODISCARD
    constexpr
    auto
    ctx() const noexcept -> native
    {
        return {.value = value()};
    }

public:
    constexpr
    reloc_entry() noexcept = default;

    explicit
    constexpr
    reloc_entry(
        const u16 value
    ) noexcept
        : ctx_{.value = value}
    {}

    NODISCARD
    constexpr
    auto
    value() const noexcept -> u16
    {
        return bit::little(ctx_.value);
    }

    constexpr
    auto
    value(
        const u16 val
    ) noexcept -> reloc_entry&
    {
        ctx_.value = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    offset() const noexcept -> u16
    {
        return ctx().offset;
    }

    constexpr
    auto
    offset(
        const u16 val
    ) noexcept -> reloc_entry&
    {
        auto temp = ctx();

        temp.offset = val;

        return value(temp.value);
    }

    NODISCARD
    constexpr
    auto
    type() const noexcept -> reloc_type
    {
        return native{.value = value()}.type;
    }

    constexpr
    auto
    type(
        const reloc_type val
    ) noexcept -> reloc_entry&
    {
        auto temp = ctx();

        temp.type = val;

        return value(temp.value);
    }

private:
    native ctx_{.value = 0};
};
static_assert(sizeof(reloc_entry) == 2, "misaligned win::reloc_entry, probably because enum bitfields are not supported");

class reloc_block
{
    struct native
    {
        u32         base_rva{};
        u32         size_block{};
        reloc_entry entries[ZEN_WIN32_VAR_LEN]{};
    };

public:
    constexpr
    reloc_block() noexcept = default;

    NODISCARD
    constexpr
    auto
    base_rva() const noexcept -> u32
    {
        return bit::little(ctx_.base_rva);
    }

    constexpr
    auto
    base_rva(
        const u32 val
    ) noexcept -> reloc_block&
    {
        ctx_.base_rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_block() const noexcept -> u32
    {
        return bit::little(ctx_.size_block);
    }

    constexpr
    auto
    size_block(
        const u32 val
    ) noexcept -> reloc_block&
    {
        ctx_.size_block = bit::little(val);

        return *this;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    num_entries() const noexcept -> szt
    {
        return static_cast<szt>(end() - begin());
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    begin() noexcept -> reloc_entry*
    {
        return &ctx_.entries[0];
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    begin() const noexcept -> const reloc_entry*
    {
        return const_cast<reloc_block*>(this)->begin();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    end() noexcept -> reloc_entry*
    {
        return reinterpret_cast<reloc_entry*>(next());
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    end() const noexcept -> const reloc_entry*
    {
        return const_cast<reloc_block*>(this)->end();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    next() noexcept -> reloc_block*
    {
        return reinterpret_cast<reloc_block*>(reinterpret_cast<std::byte*>(this) + size_block());
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    next() const noexcept -> const reloc_block*
    {
        return const_cast<reloc_block*>(this)->next();
    }

private:
    native ctx_{};
};

struct reloc_directory
{
    reloc_block first_block;
};
ZEN_RESTORE_ALIGNMENT() // namespace zen::win

