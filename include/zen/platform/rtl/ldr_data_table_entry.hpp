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

#include <zen/platform/rtl/list_entry.hpp>
#include <zen/platform/rtl/string.hpp>

namespace zen::rtl {
template<bool X64 = detail::is_64_bit>
struct ldr_data_table_entry
{
    list_entry<X64>     in_load_order_links{};
    list_entry<X64>     in_memory_order_links{};
    list_entry<X64>     in_initialization_order_links{};
    va_t<X64>           dll_base{};
    va_t<X64>           entry_point{};
    u32                 size_of_image{};
    unicode_string<X64> full_dll_name{};
    unicode_string<X64> base_dll_name{};
    u32                 flags{};
    u16                 load_count{};
    u16                 tls_index{};
    list_entry<X64>     hash_links{};
    u32                 timedate_stamp{};
    va_t<X64>           entry_point_activation_context{};
    va_t<X64>           patch_information{};
};
} //namespace zen::rtl
