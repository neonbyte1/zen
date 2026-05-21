// Copyright (c) 2025 - 2026, neonbyte - All rights reserved.
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
struct api_set_namespace_entry_v2
{
    u32 name_offset{};
    u32 name_len{};
    u32 data_offset{};
};

struct api_set_value_entry_v2
{
    u32 name_offset{};
    u32 name_len{};
    u32 value_offset{};
    u32 value_len{};
};

struct api_set_namespace_entry_v4
{
    u32 flags{};
    u32 name_offset{};
    u32 name_len{};
    u32 alias_offset{};
    u32 alias_len{};
    u32 data_offset{};
};

struct api_set_value_entry_v4
{
    u32 flags{};
    u32 name_offset{};
    u32 name_len{};
    u32 value_offset{};
    u32 value_len{};
};

struct api_set_namespace_v6
{
    u32 version{};
    u32 size{};
    u32 flags{};
    u32 count{};
    u32 entry_offset{};
    u32 hash_offset{};
    u32 hash_factor{};
};

struct api_set_namespace_entry_v6
{
    u32 flags{};
    u32 name_offset{};
    u32 name_len{};
    u32 hashed_len{};
    u32 value_offset{};
    u32 value_count{};
};

struct api_set_value_entry_v6
{
    u32 flags{};
    u32 name_offset{};
    u32 name_len{};
    u32 value_offset{};
    u32 value_len{};
};
} //namespace zen::rtl
