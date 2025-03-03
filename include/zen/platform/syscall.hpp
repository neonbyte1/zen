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

#include <zen/platform/constants.hpp>
#include <zen/platform/common/status_code.hpp>
#include <zen/platform/rtl/memory_basic_information.hpp>
#include <zen/platform/rtl/process_basic_information.hpp>
#include <zen/platform/rtl/thread_basic_information.hpp>
#include <zen/platform/rtl/system_process_information.hpp>
#include <zen/platform/rtl/section_basic_information.hpp>
#include <zen/platform/rtl/object_basic_information.hpp>
#include <zen/platform/rtl/object_attributes.hpp>
#include <zen/platform/rtl/io_status_block.hpp>
#include <zen/platform/rtl/large_integer.hpp>

#if defined(ZEN_TARGET_64_BIT)
namespace zen::detail {
extern "C" void* syscall_stub();

template<class T>
using to_int64 = std::conditional_t<sizeof(T) < sizeof(i64), i64, T> ;
} //namespace zen::detail
#endif

namespace zen::win {
#if defined(ZEN_TARGET_64_BIT)
NODISCARD
auto
get_syscall_id(
    uptr module_handle,
    u32  name,
    bool lowercase = true
) noexcept -> u32;

NODISCARD
auto
get_syscall_id(
    uptr             module_handle,
    std::string_view name,
    bool             lowercase = true
) noexcept -> u32;

template<class Ret = status_code, class ...Args>
requires(sizeof(Ret) <= sizeof(void*))
auto
syscall(
    const u32 id,
    Args...   args
) noexcept -> Ret
{
    const auto pfn = reinterpret_cast<
        std::conditional_t<std::is_same_v<status_code, Ret>, long_t, Ret>(*)(u32, szt, detail::to_int64<Args>...)
    >(detail::syscall_stub);

    const auto result = pfn(id, sizeof...(Args), detail::to_int64<Args>(args)...);

    if constexpr (std::is_same_v<status_code, Ret>) {
        return status_code{result};
    } else {
        return result;
    }
}
#endif

auto
nt_query_information_process(
    const void*       process_handle,
    rtl::process_info id,
    void*             buffer,
    u64               size,
    u64*              return_length
) noexcept -> status_code;

auto
nt_query_information_thread(
    const void*      thread_handle,
    rtl::thread_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
nt_query_system_information(
    rtl::system_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
nt_query_object(
    const void*      handle,
    rtl::object_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
nt_query_section(
    const void*       handle,
    rtl::section_info id,
    void*             buffer,
    u64               size,
    u64*              return_length
) noexcept -> status_code;

auto
nt_query_virtual_memory(
    const void*      handle,
    u64              base_address,
    rtl::memory_info id,
    void*            buffer,
    u64              size,
    u64*             return_length
) noexcept -> status_code;

auto
nt_allocate_virtual_memory(
    const void*     handle,
    u64*            base_address,
    uptr            zero_bits,
    u64             size,
    allocation_type allocation,
    page_protection protection
) noexcept -> status_code;

auto
nt_free_virtual_memory(
    const void* handle,
    u64         base_address,
    u64         size,
    u32         free_type
) noexcept -> status_code;

auto
nt_protect_virtual_memory(
    const void*      handle,
    u64              base_address,
    u64              size,
    page_protection  new_protection,
    page_protection* old_protection
) noexcept -> status_code;

auto
nt_read_virtual_memory(
    const void* handle,
    u64         base_address,
    void*       buffer,
    u64         size,
    u64*        bytes_read
) noexcept -> status_code;

auto
nt_write_virtual_memory(
    const void* handle,
    u64         base_address,
    const void* buffer,
    u64         size,
    u64*        bytes_written
) noexcept -> status_code;

auto
nt_get_context_thread(
    const void* handle,
    void*       context
) noexcept -> status_code;

auto
nt_set_context_thread(
    const void* handle,
    const void* context
) noexcept -> status_code;

auto
nt_close(
    const void* handle
) noexcept -> status_code;

auto
nt_open_process(
    u32            pid,
    process_access desired_access,
    status_code*   returned_status = nullptr
) noexcept -> void*;

auto
nt_suspend_process(
    const void* process_handle
) noexcept -> status_code;

auto
nt_resume_process(
    const void* process_handle
) noexcept -> status_code;

auto
nt_terminate_process(
    const void* process_handle,
    long_t      exit_status
) noexcept -> status_code;

auto
nt_create_thread_ex(
    const void*               process_handle,
    rtl::thread_routine_fn    routine,
    void*                     thread_params,
    rtl::create_thread_flags  flags,
    thread_access             desired_access        = thread_access::all,
    rtl::object_attributes<>* attributes            = nullptr,
    uptr                      stack_zero_bits       = 0,
    uptr                      size_of_stack_commit  = 0,
    uptr                      size_of_stack_reserve = 0,
    void*                     bytes_buffer          = nullptr
) noexcept -> void*;

auto
nt_suspend_thread(
    const void* thread_handle,
    u32*        previous_suspend_count
) noexcept -> status_code;

auto
nt_resume_thread(
    const void* thread_handle,
    u32*        suspend_count
) noexcept -> status_code;

auto
nt_wait_for_single_object(
    const void* handle,
    bool        alertable,
    i64*        time
) noexcept -> status_code;

auto
nt_create_section(
    void**                    section_handle,
    u32                       desired_access,
    rtl::object_attributes<>* attributes,
    i64*                      maximum_size,
    u32                       page_attributes,
    u32                       section_attributes,
    void*                     file_handle
) noexcept -> status_code;

auto
nt_map_view_of_section(
    const void*          section_handle,
    const void*          process_handle,
    u64*                 base_address,
    uptr                 zero_bits,
    szt                  commit_size,
    rtl::large_integer*  section_offset,
    rtl::large_integer*  view_size,
    rtl::section_inherit inherit_disposition,
    allocation_type      allocation,
    page_protection      protection
) noexcept -> status_code;

auto
nt_map_view_of_section(
    const void*          section_handle,
    const void*          process_handle,
    u64                  size,
    allocation_type      allocation,
    page_protection      protection,
    rtl::section_inherit inherit_disposition,
    uptr                 zero_bits = 0,
    szt                  commit_size = 0,
    rtl::large_integer*  section_offset = nullptr
) noexcept -> u64;

auto
nt_unmap_view_of_section(
    const void* process_handle,
    const void* base_address
) noexcept -> status_code;

auto
nt_unmap_view_of_section(
    const void* process_handle,
    u64         base_address
) noexcept -> status_code;

auto
nt_extend_section(
    const void* section_handle,
    szt         size
) noexcept -> status_code;
} //namespace zen::win
