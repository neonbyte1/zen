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

ZEN_COFF_ALIGNMENT(zen::coff)
struct string_table
{
    union native
    {
        u32  size;
        char raw_data[ZEN_WIN32_LEN_SHORT_STR];
    } ctx{};

    constexpr
    string_table() noexcept = default;

    NODISCARD
    constexpr
    auto
    size() const noexcept -> u32
    {
        return bit::little(ctx.size);
    }

    constexpr
    auto
    size(
        const u32 val
    ) noexcept -> string_table&
    {
        ctx.size = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    raw_data() noexcept -> char*
    {
        return ctx.raw_data;
    }

    NODISCARD
    constexpr
    auto
    raw_data() const noexcept -> const char*
    {
        return const_cast<string_table*>(this)->raw_data();
    }

    NODISCARD
    constexpr
    auto
    begin() const noexcept -> const char*
    {
        return size() > 4 ? &raw_data()[0] : nullptr;
    }

    NODISCARD
    constexpr
    auto
    end() const noexcept -> const char*
    {
        return size() > 4 ? &raw_data()[size()] : nullptr;
    }

    NODISCARD
    constexpr
    auto
    resolve(
        const szt offset
    ) const noexcept -> std::string_view
    {
        if (offset > 4) {
            const auto* const start = begin() + offset;
            const char* const stop  = end();

            for (const auto* it = start; it < stop; ++it) {
                if (!*it) {
                    return {start, static_cast<szt>(it - start)};
                }
            }
        }

        return {};
    }
};

struct string
{
    union native
    {
        u64  identifier;
        char short_name[ZEN_WIN32_LEN_SHORT_STR];
        struct
        {
            /// <summary>
            ///   If non-zero, name is inlined into short_name, else has a long name.
            /// </summary>
            u32 is_short;
            /// <summary>
            ///   Offset into string table
            /// </summary>
            u32 long_name_offset;
        };
    } ctx{};

    constexpr
    string() noexcept = default;


    NODISCARD
    constexpr
    auto
    operator[](
        const szt index
    ) -> char&
    {
        return const_cast<char&>(get().at(index));
    }

    NODISCARD
    constexpr
    auto
    operator[](
        const szt index
    ) const -> const char&
    {
        return get().at(index);
    }

    NODISCARD
    constexpr
    auto
    identifier() const noexcept -> u64
    {
        return bit::little(ctx.identifier);
    }

    constexpr
    auto
    identifier(
        const u64 val
    ) noexcept -> string&
    {
        ctx.identifier = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    short_name() noexcept -> char*
    {
        return ctx.short_name;
    }

    NODISCARD
    constexpr
    auto
    short_name() const noexcept -> const char*
    {
        return const_cast<string*>(this)->short_name();
    }

    NODISCARD
    constexpr
    auto
    is_short() const noexcept -> bool
    {
        if constexpr (detail::is_little_endian) {
            return ctx.is_short != 0;
        } else {
            return native{.identifier = identifier()}.is_short != 0;
        }
    }

    constexpr
    auto
    is_short(
        const bool active
    ) noexcept -> string&
    {
        if constexpr (detail::is_little_endian) {
            ctx.is_short = active;
        } else {
            native swapped{.identifier = identifier()};

            swapped.is_short = active;

            identifier(swapped.identifier);
        }

        return *this;
    }

    NODISCARD
    constexpr
    auto
    long_name_offset() const noexcept -> u32
    {
        if constexpr (detail::is_little_endian) {
            return ctx.long_name_offset;
        } else {
            return native{.identifier = identifier()}.long_name_offset;
        }
    }

    constexpr
    auto
    long_name_offset(
        const u32 val
    ) noexcept -> string&
    {
        if constexpr (detail::is_little_endian) {
            ctx.long_name_offset = val;
        } else {
            native swapped{.identifier = identifier()};

            swapped.long_name_offset = val;

            identifier(swapped.identifier);
        }

        return *this;
    }

    NODISCARD
    constexpr
    auto
    get(
        const string_table* const table = nullptr
    ) const noexcept -> std::string_view
    {
        if (table && !is_short()) {
            return table->resolve(long_name_offset());
        }

        szt len{};

        while (len != ZEN_WIN32_LEN_SHORT_STR && short_name()[len]) {
            ++len;
        }

        return {short_name(), len};
    }

    NODISCARD
    constexpr
    auto
    equals(
        const std::string_view    str,
        const string_table* const table = nullptr
    ) const noexcept -> bool
    {
        return get(table) == str;
    }

    template<szt N>
    requires(N <= ZEN_WIN32_LEN_SHORT_STR + 1)
    NODISCARD
    constexpr
    auto
    equals_s(
        const char(&str)[N]
    ) const noexcept -> bool
    {
        if constexpr (N == 1) {
            return (!is_short() && !long_name_offset()) || (is_short() && !short_name()[0]);
        }

        if constexpr (N == ZEN_WIN32_LEN_SHORT_STR + 1) {
            return !std::memcmp(short_name(), str, N - 1);
        } else {
            return !std::memcmp(short_name(), str, N);
        }
    }
};

struct scn_string
{
    char short_name[ZEN_WIN32_LEN_SHORT_STR]{};

    constexpr
    scn_string() noexcept = default;

    template<szt N>
    requires(N < ZEN_WIN32_LEN_SHORT_STR)
    explicit
    constexpr
    scn_string(
        const char(&name)[N]
    ) noexcept
    {
        std::copy_n(name, N, short_name);
    }

    NODISCARD
    constexpr
    auto
    operator[](
        const szt index
    ) -> char&
    {
        return const_cast<char&>(string()[index]);
    }

    NODISCARD
    constexpr
    auto
    operator[](
        const szt index
    ) const -> const char&
    {
        return string()[index];
    }

    NODISCARD
    constexpr
    auto
    string(
        const string_table* const table = nullptr
    ) const noexcept -> std::string_view
    {
        if (table && short_name[0] == '/') {
            auto* end = const_cast<char*>(std::end(short_name));

            return table->resolve(static_cast<szt>(std::strtoll(short_name + 1, &end, 10)));
        }

        szt len{};

        while (len != ZEN_WIN32_LEN_SHORT_STR && short_name[len]) {
            ++len;
        }

        return {short_name, len};
    }

    NODISCARD
    constexpr
    auto
    equals(
        const std::string_view    str,
        const string_table* const table = nullptr
    ) const noexcept -> bool
    {
        return string(table) == str;
    }

    template<szt N>
    requires(N <= ZEN_WIN32_LEN_SHORT_STR + 1)
    NODISCARD
    constexpr
    auto
    equals_s(
        const char(&str)[N]
    ) const noexcept -> bool
    {
        if constexpr (N == 1) {
            return !short_name[0];
        }

        if constexpr (N == ZEN_WIN32_LEN_SHORT_STR + 1) {
            return !std::memcmp(short_name, str, ZEN_WIN32_LEN_SHORT_STR);
        } else {
            return !std::memcmp(short_name, str, N);
        }
    }
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::coff
