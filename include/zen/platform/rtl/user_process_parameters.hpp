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

#include <zen/platform/rtl/string.hpp>

namespace zen::rtl {
template<bool X64 = detail::is_64_bit>
struct rtl_current_directory
{
    unicode_string<X64> dos_path{};
    va_t<X64>           handle{};
};

template<bool X64 = detail::is_64_bit>
struct rtl_user_process_parameters
{
    u32                        maximum_length{};
    u32                        length{};
    u32                        flags{};
    u32                        debug_flags{};
    va_t<X64>                  console_handle{};
    u32                        console_flags{};
    va_t<X64>                  standard_input{};
    va_t<X64>                  standard_output{};
    va_t<X64>                  standard_error{};
    rtl_current_directory<X64> current_directory{};
    unicode_string<X64>        dll_path{};
    unicode_string<X64>        image_path_name{};
    unicode_string<X64>        command_line{};
    va_t<X64>                  environment{};
    u32                        starting_x{};
    u32                        starting_y{};
    u32                        count_x{};
    u32                        count_y{};
    u32                        count_chars_x{};
    u32                        count_chars_y{};
    u32                        fill_attribute{};
    u32                        window_flags{};
    u32                        show_window_flags{};
    unicode_string<X64>        window_title{};
    unicode_string<X64>        desktop_info{};
    unicode_string<X64>        shell_info{};
    unicode_string<X64>        runtime_data{};
};
} //namespace zen::rtl
