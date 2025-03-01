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
#include <zen/platform/syscall.hpp>
#include <zen/platform/windows.hpp>
#include <zen/platform/common/handle_guard.hpp>
#if defined(ZEN_TARGET_32_BIT)
#include <zen/platform/heavensgate.hpp>
#else
#include <intrin.h>
namespace {
NODISCARD
auto
resolve_syscall_id(
    const uptr function
) noexcept -> u32
{
    if (function) {
        const auto index = *reinterpret_cast<const i32*>(function + 4);

        if (index > 0 && index < std::numeric_limits<u16>::max()) {
            return index;
        }
    }

    return 0;
}
}

#endif

using namespace zen;

#if defined(ZEN_TARGET_64_BIT)
#define ZEN_DECL_SYSCALL_METADATA(ModuleName, FunctionName)                                            \
    static u32 syscall_index;                                                                          \
    if (!syscall_index) {                                                                              \
        constexpr static auto module_hash = zen::fnv<>::hash<true>(ModuleName);                        \
        constexpr static auto name_hash = zen::fnv<>::hash<true>(FunctionName);                        \
        syscall_index = zen::win::get_syscall_id(zen::win::get_module_handle(module_hash), name_hash); \
        if (!syscall_index) {                                                                          \
            return /*STATUS_INVALID_SYSTEM_SERVICE*/ static_cast<long>(0xC000001C);                    \
        }                                                                                              \
    }

namespace {
NODISCARD
auto
nt_open_process(
    const u32    pid,
    const u32    desired_access,
    void** const process_handle
) noexcept -> long_t
{
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtOpenProcess")

    auto                     handle{win::invalid_handle_value()};
    rtl::object_attributes<> attributes{};
    rtl::client_id<>         cid{};

    cid.unique_process = pid;

    const auto status = win::syscall(syscall_index, &handle, desired_access, &attributes, &cid);

    *process_handle = handle;

    return status;
}

auto
nt_create_thread_ex(
    void**                          output,
    const void* const               process_handle,
    const rtl::thread_routine_fn    routine,
    void* const                     thread_params,
    const rtl::create_thread_flags  flags,
    const u32                       desired_access,
    rtl::object_attributes<>* const attributes,
    const uptr                      stack_zero_bits,
    const uptr                      size_of_stack_commit,
    const uptr                      size_of_stack_reserve,
    void* const                     bytes_buffer
) noexcept -> long_t
{
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtCreateThreadEx");

    auto       handle{win::invalid_handle_value()};
    const auto status = win::syscall(
        syscall_index,
        &handle,
        desired_access,
        attributes,
        process_handle,
        routine,
        thread_params,
        flags,
        stack_zero_bits,
        size_of_stack_commit,
        size_of_stack_reserve,
        bytes_buffer
    );

    *output = handle;

    return status;
}
}

NODISCARD
auto
win::get_syscall_id(
    const uptr module_handle,
    const u32  name,
    const bool lowercase
) noexcept -> u32
{
    return resolve_syscall_id(get_proc_address(module_handle, name, lowercase));
}

NODISCARD
auto
win::get_syscall_id(
    const uptr             module_handle,
    const std::string_view name,
    const bool             lowercase
) noexcept -> u32
{
    return resolve_syscall_id(get_proc_address(module_handle, name, lowercase));
}
#endif

auto
win::get_pid() noexcept -> u32
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_get_pid();
#else
    // https://en.wikipedia.org/wiki/Win32_Thread_Information_Block#Contents_of_the_TIB_on_Windows
    // Type     Offset (x86)    Offset (x64)    Description
    // pointer 	FS:[0x20] 	    GS:[0x40] 	    Process ID (in some Windows distributions this field is used as DebugContext)
    return static_cast<u32>(__readgsqword(0x40));
#endif
}

auto
win::nt_query_information_process(
    const void* const       process_handle,
    const rtl::process_info id,
    void* const             buffer,
    const u64               size,
    u64* const              return_length
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        ret_len{};
    const auto status = x64_nt_query_information_process(
        process_handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtQueryInformationProcess")

    szt        ret_len{};
    const auto status = syscall(
        syscall_index,
        process_handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#endif
}

auto
win::nt_query_information_thread(
    const void*  const     thread_handle,
    const rtl::thread_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        ret_len{};
    const auto status = x64_nt_query_information_thread(
        thread_handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtQueryInformationThread")

    szt        ret_len{};
    const auto status = syscall(
        syscall_index,
        thread_handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#endif
}

auto
win::nt_query_system_information(
    const rtl::system_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        ret_len{};
    const auto status = x64_nt_query_system_information(
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtQuerySystemInformation")

    szt        ret_len{};
    const auto status = syscall(
        syscall_index,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#endif
}

auto
win::nt_query_object(
    const void* const      handle,
    const rtl::object_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        ret_len{};
    const auto status = x64_nt_query_object(
        handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtQueryObject")

    szt        ret_len{};
    const auto status = syscall(
        syscall_index,
        handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#endif
}

auto
win::nt_query_section(
    const void* const       handle,
    const rtl::section_info id,
    void* const             buffer,
    const u64               size,
    u64* const              return_length
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        ret_len{};
    const auto status = x64_nt_query_section(
        handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtQuerySection")

    szt        ret_len{};
    const auto status = syscall(
        syscall_index,
        handle,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#endif
}

auto
win::nt_query_virtual_memory(
    const void* const      handle,
    const u64              base_address,
    const rtl::memory_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        ret_len{};
    const auto status = x64_nt_query_virtual_memory(
        handle,
        base_address,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtQueryVirtualMemory")

    szt        ret_len{};
    const auto status = syscall(
        syscall_index,
        handle,
        base_address,
        id,
        buffer,
        size,
        &ret_len
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
#endif
}

auto
win::nt_allocate_virtual_memory(
    const void* const     handle,
    u64*                  base_address,
    const uptr            zero_bits,
    const u64             size,
    const allocation_type allocation,
    const page_protection protection
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_allocate_virtual_memory(
        handle,
        base_address,
        zero_bits,
        size,
        allocation,
        protection
    );
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtAllocateVirtualMemory")

    auto region_size{size};

    return syscall(
        syscall_index,
        handle,
        &base_address,
        zero_bits,
        &region_size,
        allocation,
        protection
    );
#endif
}

auto
win::nt_free_virtual_memory(
    const void* const handle,
    const u64         base_address,
    const u64         size,
    const u32         free_type
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_free_virtual_memory(
        handle,
        base_address,
        size,
        free_type
    );
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtFreeVirtualMemory")

    auto base_addr = reinterpret_cast<void*>(base_address);
    auto region_size{size};

    return syscall(syscall_index, handle, &base_addr, &region_size, free_type);
#endif
}

auto
win::nt_protect_virtual_memory(
    const void* const      handle,
    const u64              base_address,
    const u64              size,
    const page_protection  new_protection,
    page_protection* const old_protection
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_protect_virtual_memory(
        handle,
        base_address,
        size,
        new_protection,
        old_protection
    );
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtProtectVirtualMemory")

    auto            base_addr = reinterpret_cast<void*>(base_address);
    auto            region_size{size};
    page_protection old_prot{};

    const auto status = syscall(
        syscall_index,
        handle,
        &base_addr,
        &region_size,
        new_protection,
        &old_prot
    );

    if (old_protection) {
        *old_protection = old_prot;
    }

    return status;
#endif
}

auto
win::nt_read_virtual_memory(
    const void* const handle,
    const u64         base_address,
    void* const       buffer,
    const u64         size,
    u64* const        bytes_read
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        bytes_processed{};
    const auto status = x64_nt_read_virtual_memory(
        handle,
        base_address,
        buffer,
        size,
        &bytes_processed
    );

    if (bytes_read) {
        *bytes_read = static_cast<szt>(bytes_processed);
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtReadVirtualMemory")

    szt        bytes_processed{};
    const auto status = syscall(
        syscall_index,
        handle,
        base_address,
        buffer,
        size,
        &bytes_processed
    );

    if (bytes_read) {
        *bytes_read = bytes_processed;
    }

    return status;
#endif
}

auto
win::nt_write_virtual_memory(
    const void* const handle,
    const u64         base_address,
    const void* const buffer,
    const u64         size,
    u64* const        bytes_written
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        bytes_processed{};
    const auto status = x64_nt_write_virtual_memory(
        handle,
        base_address,
        buffer,
        size,
        &bytes_processed
    );

    if (bytes_written) {
        *bytes_written = static_cast<szt>(bytes_processed);
    }

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtWriteVirtualMemory")

    szt        bytes_processed{};
    const auto status = syscall(
        syscall_index,
        handle,
        base_address,
        buffer,
        size,
        &bytes_processed
    );

    if (bytes_written) {
        *bytes_written = bytes_processed;
    }

    return status;
#endif
}

auto
win::nt_get_context_thread(
    const void* const handle,
    void* const       context
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_get_context_thread(handle, context);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtGetContextThread")

    return syscall(syscall_index, handle, context);
#endif
}

auto
win::nt_set_context_thread(
    const void* const handle,
    const void* const context
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_set_context_thread(handle, context);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtSetContextThread")

    return syscall(syscall_index, handle, context);
#endif
}

auto
win::nt_close(
    const void* const handle
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_close(handle);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtClose")

    return is_valid_handle(handle)
        ? syscall<long_t>(syscall_index, handle)
        : 0;
#endif
}

auto
win::nt_open_process(
    const u32            pid,
    const process_access desired_access,
    long_t* const        returned_status
) noexcept -> void*
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_open_process(pid, desired_access, returned_status);
#else
    void*      handle{};
    const auto status = ::nt_open_process(pid, std::to_underlying(desired_access), &handle);

    if (returned_status) {
        *returned_status = status;
    }

    return handle;
#endif
}

auto
win::nt_suspend_process(
    const void* process_handle
) noexcept -> long_t
{
    if (!is_valid_handle(process_handle)) {
        return 0;
    }

#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_suspend_process(process_handle);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtSuspendProcess")

    return syscall(syscall_index, process_handle);
#endif
}

auto
win::nt_resume_process(
    const void* process_handle
) noexcept -> long_t
{
    if (!is_valid_handle(process_handle)) {
        return 0;
    }

#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_resume_process(process_handle);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtResumeProcess")

    return syscall(syscall_index, process_handle);
#endif
}

auto
win::nt_terminate_process(
    const void* const process_handle,
    const long_t      exit_status
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_terminate_process(process_handle, exit_status);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtTerminateProcess");

    return syscall(syscall_index, process_handle, exit_status);
#endif
}

auto
win::nt_create_thread_ex(
    const void* const               process_handle,
    const rtl::thread_routine_fn    routine,
    void* const                     thread_params,
    const rtl::create_thread_flags  flags,
    const thread_access             desired_access,
    rtl::object_attributes<>* const attributes,
    const uptr                      stack_zero_bits,
    const uptr                      size_of_stack_commit,
    const uptr                      size_of_stack_reserve,
    void* const                     bytes_buffer
) noexcept -> void*
{
#if defined(ZEN_TARGET_32_BIT)
    rtl::object_attributes<true> x64_obj_attributes{};
    rtl::unicode_string<true>    x64_obj_name{};

    if (attributes) {
        x64_obj_attributes.root_directory              = attributes->root_directory;
        x64_obj_attributes.attributes                  = attributes->attributes;
        x64_obj_attributes.security_descriptor         = attributes->security_descriptor;
        x64_obj_attributes.security_quality_of_service = attributes->security_quality_of_service;

        if (attributes->object_name) {
            x64_obj_name.len(attributes->object_name->len());
            x64_obj_name.max_len(attributes->object_name->max_len());
            x64_obj_name.buffer(attributes->object_name->buffer());

            x64_obj_attributes.object_name = &x64_obj_name;
        }
    }

    return x64_nt_create_thread_ex(
        process_handle,
        reinterpret_cast<u64>(routine),
        reinterpret_cast<u64>(thread_params),
        flags,
        desired_access,
        attributes ? &x64_obj_attributes : nullptr,
        stack_zero_bits,
        size_of_stack_commit,
        size_of_stack_reserve,
        bytes_buffer
    );
#else
    void* handle{};

    ::nt_create_thread_ex(
        &handle,
        process_handle,
        routine,
        thread_params,
        flags,
        std::to_underlying(desired_access),
        attributes,
        stack_zero_bits,
        size_of_stack_commit,
        size_of_stack_reserve,
        bytes_buffer
    );

    return handle;
#endif
}

auto
win::nt_suspend_thread(
    const void* thread_handle,
    u32*        previous_suspend_count
) noexcept -> long_t
{
    if (!is_valid_handle(thread_handle)) {
        return 0;
    }

#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_suspend_thread(thread_handle, previous_suspend_count);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtSuspendThread")

    u32        count{};
    const auto status = syscall(syscall_index, thread_handle, &count);

    if (previous_suspend_count) {
        *previous_suspend_count = count;
    }

    return status;
#endif
}

auto
win::nt_resume_thread(
    const void* thread_handle,
    u32*        suspend_count
) noexcept -> long_t
{
    if (!is_valid_handle(thread_handle)) {
        return 0;
    }

#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_resume_thread(thread_handle, suspend_count);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtResumeThread")

    return syscall(syscall_index, thread_handle, suspend_count);
#endif
}

auto
win::nt_wait_for_single_object(
    const void* const handle,
    const bool        alertable,
    i64* const        time
) noexcept -> long_t
{
    if (!is_valid_handle(handle)) {
        return 0;
    }

#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_wait_for_single_object(handle, alertable, time);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtWaitForSingleObject")

    return syscall(syscall_index, alertable, time);
#endif
}

auto
win::nt_create_section(
    void** const                    section_handle,
    const u32                       desired_access,
    rtl::object_attributes<>* const attributes,
    i64* const                      maximum_size,
    const u32                       page_attributes,
    const u32                       section_attributes,
    void* const                     file_handle
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    rtl::object_attributes<true> x64_obj_attributes{};
    rtl::unicode_string<true>    x64_obj_name{};

    if (attributes) {
        x64_obj_attributes.root_directory              = attributes->root_directory;
        x64_obj_attributes.attributes                  = attributes->attributes;
        x64_obj_attributes.security_descriptor         = attributes->security_descriptor;
        x64_obj_attributes.security_quality_of_service = attributes->security_quality_of_service;

        if (attributes->object_name) {
            x64_obj_name.len(attributes->object_name->len());
            x64_obj_name.max_len(attributes->object_name->max_len());
            x64_obj_name.buffer(attributes->object_name->buffer());

            x64_obj_attributes.object_name = &x64_obj_name;
        }
    }

    return x64_nt_create_section(
        section_handle,
        desired_access,
        attributes ? &x64_obj_attributes : nullptr,
        maximum_size,
        page_attributes,
        section_attributes,
        file_handle
    );
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtCreateSection");

    auto*      handle{invalid_handle_value()};
    const auto status = syscall(
        syscall_index,
        &handle,
        desired_access,
        attributes,
        maximum_size,
        page_attributes,
        section_attributes,
        file_handle
    );

    *section_handle = handle;

    return status;
#endif
}

auto
win::nt_map_view_of_section(
    const void* const          section_handle,
    const void* const          process_handle,
    void** const               base_address,
    const uptr                 zero_bits,
    const szt                  commit_size,
    i64* const                 section_offset,
    szt* const                 view_size,
    const rtl::section_inherit inherit_disposition,
    const allocation_type      allocation,
    const page_protection      protection
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    u64        base_addr{};
    auto       size   = static_cast<u64>(view_size ? *view_size : 0);
    const auto status = x64_nt_map_view_of_section(
        section_handle,
        process_handle,
        &base_addr,
        zero_bits,
        commit_size,
        section_offset,
        &size,
        inherit_disposition,
        allocation,
        protection
    );

    *base_address = reinterpret_cast<void*>(base_address);

    return status;
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtMapViewOfSection")

    return syscall(
        syscall_index,
        section_handle,
        process_handle,
        base_address,
        zero_bits,
        commit_size,
        section_offset,
        view_size,
        inherit_disposition,
        allocation,
        protection
    );
#endif
}

auto
win::nt_map_view_of_section(
    const void* const          section_handle,
    const void* const          process_handle,
    szt                        size,
    const allocation_type      allocation,
    const page_protection      protection,
    const rtl::section_inherit inherit_disposition,
    const uptr                 zero_bits,
    const szt                  commit_size,
    i64* const                 section_offset
) noexcept -> u64
{
    void* base_address{};

    nt_map_view_of_section(
        section_handle,
        process_handle,
        &base_address,
        zero_bits,
        commit_size,
        section_offset,
        &size,
        inherit_disposition,
        allocation,
        protection
    );

    return reinterpret_cast<uptr>(base_address);
}

auto
win::nt_unmap_view_of_section(
    const void* const process_handle,
    const void* const base_address
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_unmap_view_of_section(
        process_handle,
        reinterpret_cast<const u64>(base_address)
    );
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtUnmapViewOfSection")

    return syscall(syscall_index, process_handle, base_address);
#endif
}

auto
win::nt_unmap_view_of_section(
    const void* const process_handle,
    const u64         base_address
) noexcept -> long_t
{
    return nt_unmap_view_of_section(
        process_handle,
        reinterpret_cast<const void*>(base_address)
    );
}

auto
win::nt_extend_section(
    const void* const section_handle,
    const szt         size
) noexcept -> long_t
{
#if defined(ZEN_TARGET_32_BIT)
    return x64_nt_extend_section(section_handle, size);
#else
    ZEN_DECL_SYSCALL_METADATA("ntdll.dll", "NtExtendSection")

    auto section_size = static_cast<iptr>(size);

    return syscall(syscall_index, section_handle, &section_size);
#endif
}

auto
detail::nt_close_guard::close(
    const void* const handle
) noexcept -> void
{
    win::nt_close(handle);
}
