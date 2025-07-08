// Copyright (c) 2014 ReWolf
//               2025 neonbyte
//
// http://blog.rewolf.pl/
// https://github.com/neonbyte1
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
#pragma once

#include <zen/platform/constants.hpp>
#include <zen/platform/common/status_code.hpp>
#include <zen/platform/rtl/memory_basic_information.hpp>
#include <zen/platform/rtl/process_basic_information.hpp>
#include <zen/platform/rtl/thread_basic_information.hpp>
#include <zen/platform/rtl/system_process_information.hpp>
#include <zen/platform/rtl/object_basic_information.hpp>
#include <zen/platform/rtl/section_basic_information.hpp>
#include <zen/platform/rtl/io_status_block.hpp>
#include <zen/platform/rtl/object_attributes.hpp>
#include <zen/platform/rtl/large_integer.hpp>

#if !defined(ZEN_OS_WINDOWS)
#   pragma warning "You're including the HeavensGate header which is only for Windows"
#elif defined(ZEN_TARGET_32_BIT)
#if !defined(EMIT)
#   define EMIT(a) __asm __emit (a)
#endif //EMIT

#if !defined(X64_Start_with_CS)
#   define X64_Start_with_CS(_cs) \
        { \
        EMIT(0x6A) EMIT(_cs)                         /*  push   _cs             */ \
        EMIT(0xE8) EMIT(0) EMIT(0) EMIT(0) EMIT(0)   /*  call   $+5             */ \
        EMIT(0x83) EMIT(4) EMIT(0x24) EMIT(5)        /*  add    dword [esp], 5  */ \
        EMIT(0xCB)                                   /*  retf                   */ \
        }
#endif //X64_Start_with_CS

#if !defined(X64_End_with_CS)
#   define X64_End_with_CS(_cs) \
        { \
        EMIT(0xE8) EMIT(0) EMIT(0) EMIT(0) EMIT(0)                                 /*  call   $+5                   */ \
        EMIT(0xC7) EMIT(0x44) EMIT(0x24) EMIT(4) EMIT(_cs) EMIT(0) EMIT(0) EMIT(0) /*  mov    dword [rsp + 4], _cs  */ \
        EMIT(0x83) EMIT(4) EMIT(0x24) EMIT(0xD)                                    /*  add    dword [rsp], 0xD      */ \
        EMIT(0xCB)                                                                 /*  retf                         */ \
        }
#endif //X64_End_with_CS

#if !defined(X64_Start)
#   define X64_Start() X64_Start_with_CS(0x33)
#endif //X64_Start

#if !defined(X64_End)
#   define X64_End() X64_End_with_CS(0x23)
#endif //X64_End

#if !defined(X64_Push)
#   define X64_Push(r) EMIT(0x48 | ((r) >> 3)) EMIT(0x50 | ((r) & 7))
#endif //X64_Push

#if !defined(X64_Pop)
#   define X64_Pop(r) EMIT(0x48 | ((r) >> 3)) EMIT(0x58 | ((r) & 7))
#endif //X64_Pop

#if !defined(X64_REX_W)
#   define X64_REX_W EMIT(0x48) __asm
#endif //X64_REX_W

#if !defined(_RAX)
#   define X64_RAX 0
#endif //X64_RAX

#if !defined(_RCX)
#   define X64_RCX 1
#endif //X64_RCX

#if !defined(_RDX)
#   define X64_RDX 2
#endif //X64_RDX

#if !defined(_RBX)
#   define X64_RBX 3
#endif //X64_RBX

#if !defined(_RSP)
#   define X64_RSP 4
#endif //X64_RSP

#if !defined(_RBP)
#   define X64_RBP 5
#endif //X64_RBP

#if !defined(_RSI)
#   define X64_RSI 6
#endif //X64_RSI

#if !defined(_RDI)
#   define X64_RDI 7
#endif //X64_RDI

#if !defined(X64_R8)
#   define X64_R8 8
#endif //X64_R8

#if !defined(X64_R9)
#   define X64_R9 9
#endif //X64_R9

#if !defined(_R10)
#   define X64_R10 10
#endif //X64_R10

#if !defined(_R11)
#   define X64_R11 11
#endif //X64_R11

#if !defined(_R12)
#   define X64_R12 12
#endif //X64_R12

#if !defined(_R13)
#   define X64_R13 13
#endif //X64_R13

#if !defined(_R14)
#   define X64_R14 14
#endif //X64_R14

#if !defined(_R15)
#   define X64_R15 15
#endif //X64_R15

namespace zen::win {
NODISCARD
constexpr
auto
make_reg64(
    const u64 value
) noexcept -> b8
{
    return {.u64 = value};
}

template<class T>
NODISCARD
constexpr
auto
x64_from_handle(
    const T val
) noexcept -> u64
{

    // Sign-extension is required for pseudo handles such as the handle returned from GetCurrentProcess().
    //
    // "64-bit versions of Windows use 32-bit handles for interoperability [...] it is
    // safe to [...] sign-extend the handle (when passing it from 32-bit to 64-bit)."
    // https://docs.microsoft.com/en-us/windows/win32/winprog64/interprocess-communication

    if constexpr (std::is_pointer_v<T>) {
        return static_cast<u64>(reinterpret_cast<const iptr>(val));
    } else {
        return static_cast<u64>(static_cast<iptr>(val));
    }
}

template<class T>
NODISCARD
constexpr
auto
x64_from_ptr(
    const T val
) noexcept -> u64
{
    // Without the double casting, the pointer is sign extended, not zero extended,
    // which leads to invalid addresses with /LARGEADDRESSAWARE.

    if constexpr (std::is_pointer_v<T>) {
        return static_cast<u64>(reinterpret_cast<const uptr>(val));
    } else {
        return static_cast<u64>(static_cast<uptr>(val));
    }
}

NODISCARD
auto
x64_get_peb() noexcept -> u64;

NODISCARD
auto
x64_get_pid() noexcept -> u32;

NODISCARD
auto
x64_get_tid() noexcept -> u32;

auto
__cdecl
x64_call(
    u64 function,
    szt num_arguments,
    ...
) noexcept -> u64;

auto
x64_memcpy(
    void* dst,
    u64   src,
    szt   num_bytes
) noexcept -> void;

auto
x64_memcmp(
    const void* dst,
    u64         src,
    szt         num_bytes
) noexcept -> bool;

NODISCARD
auto
x64_get_module_handle(
    u32  name,
    bool lowercase = true
) noexcept -> u64;

NODISCARD
auto
x64_get_module_handle(
    std::string_view name,
    bool             lowercase = true
) noexcept -> u64;

NODISCARD
auto
x64_get_module_handle(
    std::wstring_view name,
    bool              lowercase = true
) noexcept -> u64;

NODISCARD
auto
x64_get_proc_address(
    u64  handle,
    u32  name,
    bool lowercase = true
) noexcept -> u64;

NODISCARD
auto
x64_get_proc_address(
    u64              handle,
    std::string_view name,
    bool             lowercase = true
) noexcept -> u64;

auto
x64_nt_query_information_process(
    const void*       process_handle,
    rtl::process_info id,
    void*             buffer,
    u64               size,
    u64*              return_length
) noexcept -> status_code;

auto
x64_nt_query_information_thread(
    const void*      thread_handle,
    rtl::thread_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
x64_nt_query_system_information(
    rtl::system_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
x64_nt_query_object(
    const void*      handle,
    rtl::object_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
x64_nt_query_section(
    const void*       handle,
    rtl::section_info id,
    void*             buffer,
    u64               size,
    u64*              return_length
) noexcept -> status_code;

auto
x64_nt_query_virtual_memory(
    const void*      handle,
    u64              base_address,
    rtl::memory_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
x64_nt_allocate_virtual_memory(
    const void*     handle,
    u64*            base_address,
    u64             zero_bits,
    u64             size,
    allocation_type allocation,
    page_protection protection
) noexcept -> status_code;

auto
x64_nt_free_virtual_memory(
    const void* handle,
    u64         base_address,
    u64         size,
    u32         free_type
) noexcept -> status_code;

auto
x64_nt_protect_virtual_memory(
    const void*      handle,
    u64              base_address,
    u64              size,
    page_protection  new_protection,
    page_protection* old_protection
) noexcept -> status_code;

auto
x64_nt_read_virtual_memory(
    const void* handle,
    u64         base_address,
    void*       buffer,
    u64         size,
    u64*        bytes_read
) noexcept -> status_code;

auto
x64_nt_write_virtual_memory(
    const void* handle,
    u64         base_address,
    const void* buffer,
    u64         size,
    u64*        bytes_written
) noexcept -> status_code;

auto
x64_nt_get_context_thread(
    const void* handle,
    void*       context
) noexcept -> status_code;

auto
x64_nt_set_context_thread(
    const void* handle,
    const void* context
) noexcept -> status_code;

auto
x64_nt_close(
    const void* handle
) noexcept -> status_code;

NODISCARD
auto
x64_nt_open_process(
    u32            pid,
    process_access desired_access,
    status_code*   returned_status = nullptr
) noexcept -> void*;

auto
x64_nt_suspend_process(
    const void* process_handle
) noexcept -> status_code;

auto
x64_nt_resume_process(
    const void* process_handle
) noexcept -> status_code;

auto
x64_nt_terminate_process(
    const void* process_handle,
    long_t      exit_status
) noexcept -> status_code;

auto
x64_nt_create_thread_ex(
    const void*                   process_handle,
    u64                           routine,
    u64                           thread_params,
    rtl::create_thread_flags      flags,
    thread_access                 desired_access        = thread_access::all,
    rtl::object_attributes<true>* attributes            = nullptr,
    u64                           stack_zero_bits       = 0,
    u64                           size_of_stack_commit  = 0,
    u64                           size_of_stack_reserve = 0,
    void*                         bytes_buffer          = nullptr
) noexcept -> void*;

NODISCARD
auto
x64_nt_open_thread(
    u32           thread_id,
    thread_access desired_access,
    status_code*  returned_status = nullptr
) noexcept -> void*;

auto
x64_nt_suspend_thread(
    const void* thread_handle,
    u32*        previous_suspend_count
) noexcept -> status_code;

auto
x64_nt_resume_thread(
    const void* thread_handle,
    u32*        suspend_count
) noexcept -> status_code;

auto
x64_nt_wait_for_single_object(
    const void* handle,
    bool        alertable,
    i64*        time
) noexcept -> status_code;

auto
x64_nt_create_section(
    void**                        section_handle,
    u32                           desired_access,
    rtl::object_attributes<true>* attributes,
    i64*                          maximum_size,
    u32                           page_attributes,
    u32                           section_attributes,
    void*                         file_handle
) noexcept -> status_code;

auto
x64_nt_map_view_of_section(
    const void*          section_handle,
    const void*          process_handle,
    u64*                 base_address,
    u64                  zero_bits,
    u64                  commit_size,
    rtl::large_integer*  section_offset,
    rtl::large_integer*  view_size,
    rtl::section_inherit inherit_disposition,
    allocation_type      allocation,
    page_protection      protection
) noexcept -> status_code;

auto
x64_nt_map_view_of_section(
    const void*          section_handle,
    const void*          process_handle,
    u64                  size,
    allocation_type      allocation,
    page_protection      protection,
    rtl::section_inherit inherit_disposition,
    u64                  zero_bits = 0,
    u64                  commit_size = 0,
    rtl::large_integer*  section_offset = nullptr
) noexcept -> u64;

auto
x64_nt_unmap_view_of_section(
    const void* process_handle,
    u64         base_address
) noexcept -> status_code;

auto
x64_nt_extend_section(
    const void* section_handle,
    u64         size
) noexcept -> status_code;

auto
x64_nt_user_get_key_state(
    i32 key
) noexcept -> i16;

auto
x64_nt_user_get_async_key_state(
    i32 key
) noexcept -> i16;
} //namespace zen::win
#endif
