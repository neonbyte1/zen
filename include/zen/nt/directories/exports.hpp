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

#include <zen/coff/version.hpp>

ZEN_WIN32_ALIGNMENT(zen::win)
class export_directory
{
    struct native
    {
        uint32_t  characteristics{};
        uint32_t  timedate_stamp{};
        version32 version{};
        uint32_t  name{};
        uint32_t  base{};
        uint32_t  num_functions{};
        uint32_t  num_names{};
        uint32_t  rva_functions{};
        uint32_t  rva_names{};
        uint32_t  rva_name_ordinals{};
    };

public:
    constexpr
    export_directory() noexcept = default;

    NODISCARD
    constexpr
    auto
    characteristics() const noexcept -> u32
    {
        return bit::little(ctx_.characteristics);
    }

    constexpr
    auto
    characteristics(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.characteristics = bit::little(val);

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
    ) noexcept -> export_directory&
    {
        ctx_.timedate_stamp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    version() noexcept -> version32&
    {
        return ctx_.version;
    }

    NODISCARD
    constexpr
    auto
    version() const noexcept -> const version32&
    {
        return const_cast<export_directory*>(this)->version();
    }

    NODISCARD
    constexpr
    auto
    name() const noexcept -> u32
    {
        return bit::little(ctx_.name);
    }

    constexpr
    auto
    name(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.name = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    base() const noexcept -> u32
    {
        return bit::little(ctx_.base);
    }

    constexpr
    auto
    base(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.base = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_functions() const noexcept -> u32
    {
        return bit::little(ctx_.num_functions);
    }

    constexpr
    auto
    num_functions(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.num_functions = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_names() const noexcept -> u32
    {
        return bit::little(ctx_.num_names);
    }

    constexpr
    auto
    num_names(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.num_names = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_functions() const noexcept -> u32
    {
        return bit::little(ctx_.rva_functions);
    }

    constexpr
    auto
    rva_functions(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.rva_functions = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_names() const noexcept -> u32
    {
        return bit::little(ctx_.rva_names);
    }

    constexpr
    auto
    rva_names(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.rva_names = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_name_ordinals() const noexcept -> u32
    {
        return bit::little(ctx_.rva_name_ordinals);
    }

    constexpr
    auto
    rva_name_ordinals(
        const u32 val
    ) noexcept -> export_directory&
    {
        ctx_.rva_name_ordinals = bit::little(val);

        return *this;
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
