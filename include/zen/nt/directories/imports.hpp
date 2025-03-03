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
class import_directory
{
    struct native
    {
        union
        {
            u32 characteristics;                // 0 for terminating null import descriptor
            u32 rva_original_first_thunk;       // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
        };
        u32 timedate_stamp;                     // 0 if not bound,
                                                // -1 if bound, and real date    ime stamp
                                                //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
                                                // O.W. date/time stamp of DLL bound to (Old BIND)
        u32 forwarder_chain;                    // -1 if no forwarders
        u32 rva_name;
        u32 rva_first_thunk;                    // RVA to IAT (if bound this IAT has actual addresses)
    };

public:
    constexpr
    import_directory() noexcept = default;

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
    ) noexcept -> import_directory&
    {
        ctx_.characteristics = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_original_first_thunk() const noexcept -> u32
    {
        return bit::little(ctx_.rva_original_first_thunk);
    }

    constexpr
    auto
    rva_original_first_thunk(
        const u32 val
    ) noexcept -> import_directory&
    {
        ctx_.rva_original_first_thunk = bit::little(val);

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
    ) noexcept -> import_directory&
    {
        ctx_.timedate_stamp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    forwarder_chain() const noexcept -> u32
    {
        return bit::little(ctx_.forwarder_chain);
    }

    constexpr
    auto
    forwarder_chain(
        const u32 val
    ) noexcept -> import_directory&
    {
        ctx_.forwarder_chain = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_name() const noexcept -> u32
    {
        return bit::little(ctx_.rva_name);
    }

    constexpr
    auto
    rva_name(
        const u32 val
    ) noexcept -> import_directory&
    {
        ctx_.rva_name = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_first_thunk() const noexcept -> u32
    {
        return bit::little(ctx_.rva_first_thunk);
    }

    constexpr
    auto
    rva_first_thunk(
        const u32 val
    ) noexcept -> import_directory&
    {
        ctx_.rva_first_thunk = bit::little(val);

        return *this;
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
