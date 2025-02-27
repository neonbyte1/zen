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

#include <zen/platform/rtl/ldr_data_table_entry.hpp>
#include <zen/platform/rtl/peb_ldr_data.hpp>
#include <zen/platform/rtl/user_process_parameters.hpp>

namespace zen::rtl {
template<bool X64 = detail::is_64_bit>
struct peb
{
private:
    NODISCARD
    constexpr
    static
    auto
    size_pad1() noexcept -> szt
    {
        return X64 ? 0xC : 0x4;
    }

    NODISCARD
    constexpr
    static
    auto
    size_pad2() noexcept -> szt
    {
        return X64 ? 0x30 : 0x1C;
    }

public:
    u8 inherited_address_space{};
    u8 read_image_file_exec_options{};
    u8 begin_debugged{};

    union
    {
        u8 bit_field;
        struct
        {
            u8 image_uses_large_pages          : 1;
            u8 is_protected_process            : 1;
            u8 is_image_dynamically_relocated  : 1;
            u8 skip_patching_user32_forwarders : 1;
            u8 is_packaged_process             : 1;
            u8 is_app_container                : 1;
            u8 is_protected_process_light      : 1;
            u8 is_long_path_aware_process      : 1;
        };
    };

    PAD(size_pad1());
    va_t<X64> image_base{};
    va_t<X64> ldr{};
    va_t<X64> process_parameters{};
    va_t<X64> sub_system_data{};
    va_t<X64> process_heap{};
    PAD(size_pad2());
    va_t<X64> api_set_map{};

    NODISCARD
    auto
    loader() noexcept -> peb_ldr_data<X64>*
    {
        return reinterpret_cast<peb_ldr_data<X64>*>(ldr);
    }

    NODISCARD
    auto
    loader() const noexcept -> const peb_ldr_data<X64>*
    {
        return const_cast<peb*>(this)->loader();
    }

    NODISCARD
    auto
    params() noexcept -> rtl_user_process_parameters<X64>*
    {
        return reinterpret_cast<rtl_user_process_parameters<X64>*>(process_parameters);
    }

    NODISCARD
    auto
    params() const noexcept -> const rtl_user_process_parameters<X64>*
    {
        return const_cast<peb*>(this)->params();
    }

    NODISCARD
    auto
    api() noexcept -> void*
    {
        return reinterpret_cast<void*>(api_set_map);
    }

    NODISCARD
    auto
    api() const noexcept -> const void*
    {
        return const_cast<peb*>(this)->api();
    }
};
} //namespace zen::rtl
