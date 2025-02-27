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
class delay_load_attributes
{
    union native
    {
        u32 flags;

        struct
        {
            u32 rva_based : 1;
            u32 reserved  : 31;
        };
    };

public:
    constexpr
    delay_load_attributes() noexcept = default;

    explicit
    constexpr
    delay_load_attributes(
        const u32 flags
    ) noexcept
        : ctx_{.flags = flags}
    {}

    NODISCARD
    constexpr
    auto
    flags() const noexcept -> u32
    {
        return bit::little(ctx_.flags);
    }

    constexpr
    auto
    flags(
        const u32 val
    ) noexcept -> delay_load_attributes&
    {
        ctx_.flags = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    rva_based() const noexcept -> bool
    {
        return native{.flags = flags()}.rva_based > 0;
    }

private:
    native ctx_{};
};

class delay_load_directory
{
    struct native
    {
        delay_load_attributes attributes{};
        u32                   dll_name_rva{};
        u32                   module_handle_rva{};
        u32                   import_address_table_rva{};
        u32                   import_name_table_rva{};
        u32                   bound_import_address_table_rva{};
        u32                   unload_information_table_rva{};
        u32                   timedate_stamp{};
    };

public:
    constexpr
    delay_load_directory() noexcept = default;

    NODISCARD
    constexpr
    auto
    attributes() noexcept -> delay_load_attributes&
    {
        return ctx_.attributes;
    }

    NODISCARD
    constexpr
    auto
    attributes() const noexcept -> const delay_load_attributes&
    {
        return const_cast<delay_load_directory*>(this)->attributes();
    }

    NODISCARD
    constexpr
    auto
    dll_name_rva() const noexcept -> u32
    {
        return bit::little(ctx_.dll_name_rva);
    }

    constexpr
    auto
    dll_name_rva(
        const u32 val
    ) noexcept -> delay_load_directory&
    {
        ctx_.dll_name_rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    module_handle_rva() const noexcept -> u32
    {
        return bit::little(ctx_.module_handle_rva);
    }

    constexpr
    auto
    module_handle_rva(
        const u32 val
    ) noexcept -> delay_load_directory&
    {
        ctx_.module_handle_rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    import_address_table_rva() const noexcept -> u32
    {
        return bit::little(ctx_.import_address_table_rva);
    }

    constexpr
    auto
    import_address_table_rva(
        const u32 val
    ) noexcept -> delay_load_directory&
    {
        ctx_.import_address_table_rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    import_name_table_rva() const noexcept -> u32
    {
        return bit::little(ctx_.import_name_table_rva);
    }

    constexpr
    auto
    import_name_table_rva(
        const u32 val
    ) noexcept -> delay_load_directory&
    {
        ctx_.import_name_table_rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    bound_import_address_table_rva() const noexcept -> u32
    {
        return bit::little(ctx_.bound_import_address_table_rva);
    }

    constexpr
    auto
    bound_import_address_table_rva(
        const u32 val
    ) noexcept -> delay_load_directory&
    {
        ctx_.bound_import_address_table_rva = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    unload_information_table_rva() const noexcept -> u32
    {
        return bit::little(ctx_.unload_information_table_rva);
    }

    constexpr
    auto
    unload_information_table_rva(
        const u32 val
    ) noexcept -> delay_load_directory&
    {
        ctx_.unload_information_table_rva = bit::little(val);

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
    ) noexcept -> delay_load_directory&
    {
        ctx_.timedate_stamp = bit::little(val);

        return *this;
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
