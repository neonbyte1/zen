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

#include <zen/nt/data_directory.hpp>

ZEN_WIN32_ALIGNMENT(zen::win)
enum struct directory : szt
{
    exports        = 0,  // Export Directory
    imports        = 1,  // Import Directory
    resource       = 2,  // Resource Directory
    exception      = 3,  // Exception Directory
    security       = 4,  // Security Directory
    basereloc      = 5,  // Base Relocation Table
    debug          = 6,  // Debug Directory
    copyright      = 7,  // (X86 usage)
    architecture   = 7,  // Architecture Specific Data
    globalptr      = 8,  // RVA of GP
    tls            = 9,  // TLS Directory
    load_config    = 10, // Load Configuration Directory
    bound_import   = 11, // Bound Import Directory in headers
    iat            = 12, // Import Address Table
    delay_import   = 13, // Delay Load Import Descriptors
    com_descriptor = 14, // COM Runtime descriptor
    reserved0      = 15, // -
};

union data_directories64
{
    struct directories_t
    {
        data_directory      exports;
        data_directory      imports;
        data_directory      resources;
        data_directory      exceptions;
        raw_data_directory  security;  // File offset instead of RVA!
        data_directory      baserelocs;
        data_directory      debug;
        data_directory      architecture;
        data_directory      globalptr;
        data_directory      tls;
        data_directory      load_configs;
        data_directory      bound_imports;
        data_directory      iat;
        data_directory      delay_imports;
        data_directory      com_descriptors;
        data_directory      _reserved0;
    };

    data_directory entries[16]{};
    directories_t  dir;

    data_directories64() noexcept = default;

    NODISCARD
    constexpr
    auto
    at(
        const directory id
    ) noexcept -> data_directory&
    {
        return entries[std::to_underlying(id)];
    }

    NODISCARD
    constexpr
    auto
    at(
        const directory id
    ) const noexcept -> const data_directory&
    {
        return const_cast<data_directories64*>(this)->at(id);
    }
};

union data_directories32
{
    struct directories_t
    {
        data_directory      exports;
        data_directory      imports;
        data_directory      resources;
        data_directory      exceptions;
        raw_data_directory  security;  // File offset instead of RVA!
        data_directory      baserelocs;
        data_directory      debug;
        data_directory      copyright;
        data_directory      globalptr;
        data_directory      tls;
        data_directory      load_configs;
        data_directory      bound_imports;
        data_directory      iat;
        data_directory      delay_imports;
        data_directory      com_descriptors;
        data_directory      _reserved0;
    };

    data_directory entries[16]{};
    directories_t  dir;

    data_directories32() noexcept = default;

    NODISCARD
    constexpr
    auto
    at(
        const directory id
    ) noexcept -> data_directory&
    {
        return entries[std::to_underlying(id)];
    }

    NODISCARD
    constexpr
    auto
    at(
        const directory id
    ) const noexcept -> const data_directory&
    {
        return const_cast<data_directories32*>(this)->at(id);
    }
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
