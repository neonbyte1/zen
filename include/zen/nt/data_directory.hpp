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
class data_directory
{
    struct native
    {
        u32 rva{};
        u32 size{};
    };

public:
    constexpr
    data_directory() noexcept = default;

    NODISCARD
    constexpr
    auto
    rva() const noexcept -> u32
    {
        return bit::little(ctx_.rva);
    }

    constexpr
    auto
    rva(
        const u32 val
    ) noexcept -> data_directory&
    {
        ctx_.rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size() const noexcept -> u32
    {
        return bit::little(ctx_.size);
    }

    constexpr
    auto
    size(
        const u32 val
    ) noexcept -> data_directory&
    {
        ctx_.size = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    present() const noexcept -> bool
    {
        return rva() > 0 && size() > 0;
    }

private:
    native ctx_{};
};

class raw_data_directory
{
    struct native
    {
        u32 ptr_raw_data{};
        u32 size{};
    };

public:
    constexpr
    raw_data_directory() noexcept = default;

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
        const u32 val
    ) noexcept -> raw_data_directory&
    {
        ctx_.ptr_raw_data = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size() const noexcept -> u32
    {
        return bit::little(ctx_.size);
    }

    constexpr
    auto
    size(
        const u32 val
    ) noexcept -> raw_data_directory&
    {
        ctx_.size = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    present() const noexcept -> bool
    {
        return ptr_raw_data() > 0 && size() > 0;
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
