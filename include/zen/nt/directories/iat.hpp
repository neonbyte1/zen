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
class image_named_import
{
    struct native
    {
        u16  hint{};
        char name[ZEN_WIN32_VAR_LEN]{};
    };

public:
    constexpr
    image_named_import() noexcept = default;

    NODISCARD
    constexpr
    auto
    hint() const noexcept -> u16
    {
        return bit::little(ctx_.hint);
    }

    constexpr
    auto
    hint(
        const u16 val
    ) noexcept -> image_named_import&
    {
        ctx_.hint = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    name() noexcept -> char*
    {
        return ctx_.name;
    }

    NODISCARD
    constexpr
    auto
    name() const noexcept -> const char*
    {
        return const_cast<image_named_import*>(this)->name();
    }

private:
    native ctx_{};
};

template<bool X64 = detail::is_64_bit>
class image_thunk_data
{
    using address_type = va_t<X64>;

    struct thunk_data32
    {
        u32 ordinal    : 16;
        u32 _reserved0 : 15;
        u32 is_ordinal : 1;
    };

    struct thunk_data64
    {
        u64 ordinal    : 16;
        u64 _reserved0 : 47;
        u64 is_ordinal : 1;
    };

    union thunk_data
    {
        using meta_type = std::conditional_t<X64, thunk_data64, thunk_data32>;

        address_type value;
        meta_type    meta;
    };

    union native
    {
        address_type forwarder_string;
        address_type function;
        address_type address; // image_named_import
        thunk_data   address_of_data;
    };

public:
    constexpr
    image_thunk_data() noexcept = default;

    NODISCARD
    constexpr
    auto
    forwarder_string() const noexcept -> address_type
    {
        return bit::little(ctx_.forwarder_string);
    }

    constexpr
    auto
    forwarder_string(
        const address_type val
    ) noexcept -> image_thunk_data&
    {
        ctx_.forwarder_string = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    function() const noexcept -> address_type
    {
        return bit::little(ctx_.function);
    }

    constexpr
    auto
    function(
        const address_type val
    ) noexcept -> image_thunk_data&
    {
        ctx_.function = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    address() const noexcept -> address_type
    {
        return bit::little(ctx_.address);
    }

    constexpr
    auto
    address(
        const address_type val
    ) noexcept -> image_thunk_data&
    {
        ctx_.address = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    address_of_data() const noexcept -> address_type
    {
        return bit::little(ctx_.address_of_data.value);
    }

    constexpr
    auto
    address_of_data(
        const address_type val
    ) noexcept -> image_thunk_data&
    {
        ctx_.address_of_data.value = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    metadata() const noexcept -> typename thunk_data::meta_type
    {
        return thunk_data{.value = bit::little(ctx_.address_of_data.value)}.meta;
    }

    NODISCARD
    constexpr
    auto
    ordinal() const noexcept -> u16
    {
        return static_cast<u16>(metadata().ordinal);
    }

    NODISCARD
    constexpr
    auto
    is_ordinal() const noexcept -> bool
    {
        return metadata().is_ordinal > 0;
    }

private:
    native ctx_{};
};

class bound_forwarder_ref
{
    struct native
    {
        u32 timedate_stamp{};
        u32 offset_module_name{};
    };

public:
    constexpr
    bound_forwarder_ref() noexcept = default;

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
    ) noexcept -> bound_forwarder_ref&
    {
        ctx_.timedate_stamp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    offset_module_name() const noexcept -> u32
    {
        return bit::little(ctx_.offset_module_name);
    }

    constexpr
    auto
    offset_module_name(
        const u32 val
    ) noexcept -> bound_forwarder_ref&
    {
        ctx_.offset_module_name = bit::little(val);

        return *this;
    }

private:
    native ctx_{};
};

class bound_import_descriptor
{
    struct native
    {
        u32 timedate_stamp{};
        u16 offset_module_name{};
        u16 num_module_forwarder_refs{};
    };

public:
    constexpr
    bound_import_descriptor() noexcept = default;

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
    ) noexcept -> bound_import_descriptor&
    {
        ctx_.timedate_stamp = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    offset_module_name() const noexcept -> u16
    {
        return bit::little(ctx_.offset_module_name);
    }

    constexpr
    auto
    offset_module_name(
        const u16 val
    ) noexcept -> bound_import_descriptor&
    {
        ctx_.offset_module_name = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_module_forwarder_refs() const noexcept -> u16
    {
        return bit::little(ctx_.num_module_forwarder_refs);
    }

    constexpr
    auto
    num_module_forwarder_refs(
        const u16 val
    ) noexcept -> bound_import_descriptor&
    {
        ctx_.num_module_forwarder_refs = bit::little(val);

        return *this;
    }

private:
    native ctx_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
