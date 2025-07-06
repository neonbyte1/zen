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

#include <zen/core/requirements.hpp>

namespace zen::rtl {
enum struct memory_info : u32
{
    basic,           // MemoryBasicInformation          => MEMORY_BASIC_INFORMATION
    working_set,     // MemoryWorkingSetInformation     => MEMORY_WORKING_SET_INFORMATION
    mapped_filename, // MemoryMappedFilenameInformation => UNICODE_STRING
    region,          // MemoryRegionInformation         => MEMORY_REGION_INFORMATION
    working_set_ex,  // MemoryWorkingSetExInformation   => MEMORY_WORKING_SET_EX_INFORMATION
    shared_commit,   // MemorySharedCommitInformation   => MEMORY_SHARED_COMMIT_INFORMATION
    last_index,
};

struct memory_basic_information32
{
    u32                  base_address{};
    u32                  allocation_base{};
    u32                  allocation_protect{};
    u32                  region_size{};
    win::allocation_type state{};
    win::page_protection protect{};
    u32                  type{};

    NODISCARD
    constexpr
    auto
    has_protection(
        const win::page_protection mask
    ) const noexcept -> bool
    {
        return !!(std::to_underlying(protect) & std::to_underlying(mask));
    }
};

struct ZEN_DECLSPEC_ALIGN(16) memory_basic_information64
{
    u64                  base_address{};
    u64                  allocation_base{};
    u32                  allocation_protect{};
    u16                  partition_id{};
    u16                  alignment1{};
    u64                  region_size{};
    win::allocation_type state{};
    win::page_protection protect{};
    u32                  type{};
    u32                  alignment2{};

    NODISCARD
    constexpr
    auto
    has_protection(
        const win::page_protection mask
    ) const noexcept -> bool
    {
        return !!(std::to_underlying(protect) & std::to_underlying(mask));
    }
};

template<bool X64 = true>
using memory_basic_information
    = std::conditional_t<
        X64,
        memory_basic_information64,
        memory_basic_information32
    >;
} //namespace zen::rtl
