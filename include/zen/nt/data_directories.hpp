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

struct data_directories64
{
    union
    {
        data_directory entries[16]{};

        struct
        {
            data_directory      export_directory;
            data_directory      import_directory;
            data_directory      resource_directory;
            data_directory      exception_directory;
            raw_data_directory  security_directory;  // File offset instead of RVA!
            data_directory      basereloc_directory;
            data_directory      debug_directory;
            data_directory      architecture_directory;
            data_directory      globalptr_directory;
            data_directory      tls_directory;
            data_directory      load_config_directory;
            data_directory      bound_import_directory;
            data_directory      iat_directory;
            data_directory      delay_import_directory;
            data_directory      com_descriptor_directory;
            data_directory      _reserved0;
        };
    };

    constexpr
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

struct data_directories32
{
    union
    {
        data_directory entries[16]{};

        struct
        {
            data_directory      export_directory;
            data_directory      import_directory;
            data_directory      resource_directory;
            data_directory      exception_directory;
            raw_data_directory  security_directory;  // File offset instead of RVA!
            data_directory      basereloc_directory;
            data_directory      debug_directory;
            data_directory      copyright_directory;
            data_directory      globalptr_directory;
            data_directory      tls_directory;
            data_directory      load_config_directory;
            data_directory      bound_import_directory;
            data_directory      iat_directory;
            data_directory      delay_import_directory;
            data_directory      com_descriptor_directory;
            data_directory      _reserved0;
        };
    };

    constexpr
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
