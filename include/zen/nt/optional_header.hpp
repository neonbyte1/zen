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

#include <zen/nt/data_directories.hpp>

ZEN_WIN32_ALIGNMENT(zen::win)
enum struct sub_system : u16
{
    unknown                  = 0x0000, // Unknown subsystem.
    native                   = 0x0001, // Image doesn't require a subsystem.
    windows_gui              = 0x0002, // Image runs in the Windows GUI subsystem.
    windows_cui              = 0x0003, // Image runs in the Windows character subsystem
    os2_cui                  = 0x0005, // image runs in the OS/2 character subsystem.
    posix_cui                = 0x0007, // image runs in the Posix character subsystem.
    native_windows           = 0x0008, // image is a native Win9x driver.
    windows_ce_gui           = 0x0009, // Image runs in the Windows CE subsystem.
    efi_application          = 0x000A, //
    efi_boot_service_driver  = 0x000B, //
    efi_runtime_driver       = 0x000C, //
    efi_rom                  = 0x000D,
    xbox                     = 0x000E,
    windows_boot_application = 0x0010,
    xbox_code_catalog        = 0x0011,
};

union dll_characteristics
{
    u16 flags{};
    struct
    {
        u16 _pad0                 : 5;
        u16 high_entropy_va       : 1; // Image can handle a high entropy 64-bit virtual address space.
        u16 dynamic_base          : 1; // DLL can move.
        u16 force_integrity       : 1; // Code Integrity Image
        u16 nx_compat             : 1; // Image is NX compatible
        u16 no_isolation          : 1; // Image understands isolation and doesn't want it
        u16 no_seh                : 1; // Image does not use SEH.  No SE handler may reside in this image
        u16 no_bind               : 1; // Do not bind this image.
        u16 appcontainer          : 1; // Image should execute in an AppContainer
        u16 wdm_driver            : 1; // Driver uses WDM model
        u16 guard_cf              : 1; // Image supports Control Flow Guard.
        u16 terminal_server_aware : 1;
    };
};

class optional_header_base
{
    struct native_base
    {
        u16       magic{};
        version16 linker_version{};
        u32       size_code{};
        u32       size_init_data{};
        u32       size_uninit_data{};
        u32       entry_point{};
        u32       base_of_code{};
    };

public:
    constexpr static u16 magic_value_32_bit = 0x10B;
    constexpr static u16 magic_value_64_bit = 0x20B;

    NODISCARD
    constexpr
    auto
    magic() const noexcept -> u16
    {
        return bit::little(base_ctx_.magic);
    }

    constexpr
    auto
    magic(
        const u16 val
    ) noexcept -> optional_header_base&
    {
        base_ctx_.magic = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    linker_version() noexcept -> version16&
    {
        return base_ctx_.linker_version;
    }

    NODISCARD
    constexpr
    auto
    linker_version() const noexcept -> const version16&
    {
        return const_cast<optional_header_base*>(this)->linker_version();
    }

    NODISCARD
    constexpr
    auto
    size_code() const noexcept -> u32
    {
        return bit::little(base_ctx_.size_code);
    }

    constexpr
    auto
    size_code(
        const u32 val
    ) noexcept -> optional_header_base&
    {
        base_ctx_.size_code = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_init_data() const noexcept -> u32
    {
        return bit::little(base_ctx_.size_init_data);
    }

    constexpr
    auto
    size_init_data(
        const u32 val
    ) noexcept -> optional_header_base&
    {
        base_ctx_.size_init_data = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_uninit_data() const noexcept -> u32
    {
        return bit::little(base_ctx_.size_uninit_data);
    }

    constexpr
    auto
    size_uninit_data(
        const u32 val
    ) noexcept -> optional_header_base&
    {
        base_ctx_.size_uninit_data = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    entry_point() const noexcept -> u32
    {
        return bit::little(base_ctx_.entry_point);
    }

    constexpr
    auto
    entry_point(
        const u32 val
    ) noexcept -> optional_header_base&
    {
        base_ctx_.entry_point = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    base_of_code() const noexcept -> u32
    {
        return bit::little(base_ctx_.base_of_code);
    }

    constexpr
    auto
    base_of_code(
        const u32 val
    ) noexcept -> optional_header_base&
    {
        base_ctx_.base_of_code = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    is_32_bit() const noexcept -> bool
    {
        return magic() == magic_value_32_bit;
    }

    NODISCARD
    constexpr
    auto
    is_64_bit() const noexcept -> bool
    {
        return magic() == magic_value_64_bit;
    }

protected:
    native_base base_ctx_{};
};

class optional_header64
    : public optional_header_base
{
    struct native
    {
        u64                 image_base{};
        u32                 section_alignment{};
        u32                 file_alignment{};
        version32           os_version{};
        version32           img_version{};
        version32           subsystem_version{};
        u32                 win32_version_value{};
        u32                 size_image{};
        u32                 size_headers{};
        u32                 checksum{};
        sub_system          subsystem{};
        dll_characteristics characteristics{};
        u64                 size_stack_reserve{};
        u64                 size_stack_commit{};
        u64                 size_heap_reserve{};
        u64                 size_heap_commit{};
        u32                 ldr_flags{};
        u32                 num_data_directories{};
        data_directories64  data_directories{};
    };

public:
    NODISCARD
    constexpr
    auto
    image_base() const noexcept -> u64
    {
        return bit::little(ctx_.image_base);
    }

    constexpr
    auto
    image_base(
        const u64 val
    ) noexcept -> optional_header64&
    {
        ctx_.image_base = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    section_alignment() const noexcept -> u32
    {
        return bit::little(ctx_.section_alignment);
    }

    constexpr
    auto
    section_alignment(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.section_alignment = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    file_alignment() const noexcept -> u32
    {
        return bit::little(ctx_.file_alignment);
    }

    constexpr
    auto
    file_alignment(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.file_alignment = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    os_version() noexcept -> version32&
    {
        return ctx_.os_version;
    }

    NODISCARD
    constexpr
    auto
    os_version() const noexcept -> const version32&
    {
        return const_cast<optional_header64*>(this)->os_version();
    }

    NODISCARD
    constexpr
    auto
    img_version() noexcept -> version32&
    {
        return ctx_.img_version;
    }

    NODISCARD
    constexpr
    auto
    img_version() const noexcept -> const version32&
    {
        return const_cast<optional_header64*>(this)->img_version();
    }

    NODISCARD
    constexpr
    auto
    subsystem_version() noexcept -> version32&
    {
        return ctx_.subsystem_version;
    }

    NODISCARD
    constexpr
    auto
    subsystem_version() const noexcept -> const version32&
    {
        return const_cast<optional_header64*>(this)->subsystem_version();
    }

    NODISCARD
    constexpr
    auto
    win32_version_value() const noexcept -> u32
    {
        return bit::little(ctx_.win32_version_value);
    }

    constexpr
    auto
    win32_version_value(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.win32_version_value = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_image() const noexcept -> u32
    {
        return bit::little(ctx_.size_image);
    }

    constexpr
    auto
    size_image(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.size_image = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_headers() const noexcept -> u32
    {
        return bit::little(ctx_.size_headers);
    }

    constexpr
    auto
    size_headers(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.size_headers = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    checksum() const noexcept -> u32
    {
        return bit::little(ctx_.checksum);
    }

    constexpr
    auto
    checksum(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.checksum = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    subsystem() const noexcept -> sub_system
    {
        return bit::little(ctx_.subsystem);
    }

    constexpr
    auto
    subsystem(
        const sub_system val
    ) noexcept -> optional_header64&
    {
        ctx_.subsystem = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    characteristics() const noexcept -> dll_characteristics
    {
        return {.flags = bit::little(ctx_.characteristics.flags)};
    }

    template<class T>
    requires(std::is_same_v<dll_characteristics, T> || std::is_same_v<u16, T>)
    constexpr
    auto
    characteristics(
        const T& value
    ) noexcept -> optional_header64&
    {
        if constexpr (std::is_same_v<dll_characteristics, T>) {
            ctx_.characteristics.flags = bit::little(value.flags);
        } else {
            ctx_.characteristics.flags = bit::little(value);
        }

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_stack_reserve() const noexcept -> u64
    {
        return bit::little(ctx_.size_stack_reserve);
    }

    constexpr
    auto
    size_stack_reserve(
        const u64 val
    ) noexcept -> optional_header64&
    {
        ctx_.size_stack_reserve = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_stack_commit() const noexcept -> u64
    {
        return bit::little(ctx_.size_stack_commit);
    }

    constexpr
    auto
    size_stack_commit(
        const u64 val
    ) noexcept -> optional_header64&
    {
        ctx_.size_stack_commit = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_heap_reserve() const noexcept -> u64
    {
        return bit::little(ctx_.size_heap_reserve);
    }

    constexpr
    auto
    size_heap_reserve(
        const u64 val
    ) noexcept -> optional_header64&
    {
        ctx_.size_heap_reserve = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_heap_commit() const noexcept -> u64
    {
        return bit::little(ctx_.size_heap_commit);
    }

    constexpr
    auto
    size_heap_commit(
        const u64 val
    ) noexcept -> optional_header64&
    {
        ctx_.size_heap_commit = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ldr_flags() const noexcept -> u32
    {
        return bit::little(ctx_.ldr_flags);
    }

    constexpr
    auto
    ldr_flags(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.ldr_flags = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_data_directories() const noexcept -> u32
    {
        return bit::little(ctx_.num_data_directories);
    }

    constexpr
    auto
    num_data_directories(
        const u32 val
    ) noexcept -> optional_header64&
    {
        ctx_.num_data_directories = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    data_directories() noexcept -> data_directories64&
    {
        return ctx_.data_directories;
    }

    NODISCARD
    constexpr
    auto
    data_directories() const noexcept -> const data_directories64&
    {
        return const_cast<optional_header64*>(this)->data_directories();
    }

private:
    native ctx_{};
};

class optional_header32
    : public optional_header_base
{
    struct native
    {
        u32                 base_of_data{};
        // NT additional fields.
        u32                 image_base{};
        u32                 section_alignment{};
        u32                 file_alignment{};
        version32           os_version{};
        version32           img_version{};
        version32           subsystem_version{};
        u32                 win32_version_value{};
        u32                 size_image{};
        u32                 size_headers{};
        u32                 checksum{};
        sub_system          subsystem{};
        dll_characteristics characteristics{};
        u32                 size_stack_reserve{};
        u32                 size_stack_commit{};
        u32                 size_heap_reserve{};
        u32                 size_heap_commit{};
        u32                 ldr_flags{};
        u32                 num_data_directories{};
        data_directories32  data_directories{};
    };

public:
    NODISCARD
    constexpr
    auto
    base_of_data() const noexcept -> u32
    {
        return bit::little(ctx_.base_of_data);
    }

    constexpr
    auto
    base_of_data(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.base_of_data = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    image_base() const noexcept -> u32
    {
        return bit::little(ctx_.image_base);
    }

    constexpr
    auto
    image_base(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.image_base = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    section_alignment() const noexcept -> u32
    {
        return bit::little(ctx_.section_alignment);
    }

    constexpr
    auto
    section_alignment(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.section_alignment = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    file_alignment() const noexcept -> u32
    {
        return bit::little(ctx_.file_alignment);
    }

    constexpr
    auto
    file_alignment(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.file_alignment = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    os_version() noexcept -> version32&
    {
        return ctx_.os_version;
    }

    NODISCARD
    constexpr
    auto
    os_version() const noexcept -> const version32&
    {
        return const_cast<optional_header32*>(this)->os_version();
    }

    NODISCARD
    constexpr
    auto
    img_version() noexcept -> version32&
    {
        return ctx_.img_version;
    }

    NODISCARD
    constexpr
    auto
    img_version() const noexcept -> const version32&
    {
        return const_cast<optional_header32*>(this)->img_version();
    }

    NODISCARD
    constexpr
    auto
    subsystem_version() noexcept -> version32&
    {
        return ctx_.subsystem_version;
    }

    NODISCARD
    constexpr
    auto
    subsystem_version() const noexcept -> const version32&
    {
        return const_cast<optional_header32*>(this)->subsystem_version();
    }

    NODISCARD
    constexpr
    auto
    win32_version_value() const noexcept -> u32
    {
        return bit::little(ctx_.win32_version_value);
    }

    constexpr
    auto
    win32_version_value(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.win32_version_value = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_image() const noexcept -> u32
    {
        return bit::little(ctx_.size_image);
    }

    constexpr
    auto
    size_image(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.size_image = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_headers() const noexcept -> u32
    {
        return bit::little(ctx_.size_headers);
    }

    constexpr
    auto
    size_headers(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.size_headers = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    checksum() const noexcept -> u32
    {
        return bit::little(ctx_.checksum);
    }

    constexpr
    auto
    checksum(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.checksum = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    subsystem() const noexcept -> sub_system
    {
        return bit::little(ctx_.subsystem);
    }

    constexpr
    auto
    subsystem(
        const sub_system val
    ) noexcept -> optional_header32&
    {
        ctx_.subsystem = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    characteristics() const noexcept -> dll_characteristics
    {
        return {.flags = bit::little(ctx_.characteristics.flags)};
    }

    template<class T>
    requires(std::is_same_v<dll_characteristics, T> || std::is_same_v<u16, T>)
    constexpr
    auto
    characteristics(
        const T& value
    ) noexcept -> optional_header32&
    {
        if constexpr (std::is_same_v<dll_characteristics, T>) {
            ctx_.characteristics.flags = bit::little(value.flags);
        } else {
            ctx_.characteristics.flags = bit::little(value);
        }

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_stack_reserve() const noexcept -> u32
    {
        return bit::little(ctx_.size_stack_reserve);
    }

    constexpr
    auto
    size_stack_reserve(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.size_stack_reserve = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_stack_commit() const noexcept -> u32
    {
        return bit::little(ctx_.size_stack_commit);
    }

    constexpr
    auto
    size_stack_commit(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.size_stack_commit = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_heap_reserve() const noexcept -> u32
    {
        return bit::little(ctx_.size_heap_reserve);
    }

    constexpr
    auto
    size_heap_reserve(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.size_heap_reserve = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    size_heap_commit() const noexcept -> u32
    {
        return bit::little(ctx_.size_heap_commit);
    }

    constexpr
    auto
    size_heap_commit(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.size_heap_commit = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    ldr_flags() const noexcept -> u32
    {
        return bit::little(ctx_.ldr_flags);
    }

    constexpr
    auto
    ldr_flags(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.ldr_flags = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    num_data_directories() const noexcept -> u32
    {
        return bit::little(ctx_.num_data_directories);
    }

    constexpr
    auto
    num_data_directories(
        const u32 val
    ) noexcept -> optional_header32&
    {
        ctx_.num_data_directories = bit::little(val);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    data_directories() noexcept -> data_directories32&
    {
        return ctx_.data_directories;
    }

    NODISCARD
    constexpr
    auto
    data_directories() const noexcept -> const data_directories32&
    {
        return const_cast<optional_header32*>(this)->data_directories();
    }

private:
    native ctx_{};
};

template<bool X64 = detail::is_64_bit>
using optional_header = std::conditional_t<X64, optional_header64, optional_header32>;

static_assert(sizeof(optional_header32) == 224);
static_assert(sizeof(optional_header64) == 240);
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
