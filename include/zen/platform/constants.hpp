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

#include <zen/core/requirements.hpp>

namespace zen::win {
enum struct main_reason : u32
{
    // Alias for DLL_PROCESS_DETACH
    //
    // The DLL is being unloaded from the virtual address space of the calling process because it was loaded
    // unsuccessfully or the reference count has reached zero (the processes has either terminated or called
    // FreeLibrary one time for each time it called LoadLibrary). The lpvReserved parameter indicates whether
    // the DLL is being unloaded as a result of a FreeLibrary call, a failure to load, or process termination.
    // The DLL can use this opportunity to call the TlsFree function to free any TLS indices allocated by
    // using TlsAlloc and to free any thread local data. Note that the thread that receives the process_detach
    // notification is not necessarily the same thread that received the process_attach notification.
    process_detach = 0,

    // Alias for DLL_PROCESS_ATTACH
    //
    // The DLL is being loaded into the virtual address space of the current process as a result of the process
    // starting up or as a result of a call to LoadLibrary. DLLs can use this opportunity to initialize any
    // instance data or to use the TlsAlloc function to allocate a thread local storage (TLS) index.
    // The lpvReserved parameter indicates whether the DLL is being loaded statically or dynamically.
    process_attach,

    // Alias for DLL_THREAD_ATTACH
    //
    // The current process is creating a new thread. When this occurs, the system calls the entry-point function
    // of all DLLs currently attached to the process. The call is made in the context of the new thread.
    // DLLs can use this opportunity to initialize a TLS slot for the thread. A thread calling the DLL entry-point
    // function with process_attach does not call the DLL entry-point function with thread_attach. Note that
    // a DLL's entry-point function is called with this value only by threads created after the DLL is loaded
    // by the process. When a DLL is loaded using LoadLibrary, existing threads do not call the entry-point
    // function of the newly loaded DLL.
    thread_attach,

    // Alias for DLL_THREAD_DETACH
    //
    // A thread is exiting cleanly. If the DLL has stored a pointer to allocated memory in a TLS slot, it should
    // use this opportunity to free the memory. The system calls the entry-point function of all currently loaded
    // DLLs with this value. The call is made in the context of the exiting thread.
    thread_detach
};

enum struct allocation_type : u32
{
    extended_parameter_numa_node_mandatory      = 0x00000000,
    extended_parameter_graphics                 = 0x00000001,
    extended_parameter_nonpaged                 = 0x00000002,
    extended_parameter_zero_pages_optional      = 0x00000004,
    extended_parameter_nonpaged_large           = 0x00000008,
    extended_parameter_nonpaged_huge            = 0x00000010,
    extended_parameter_soft_fault_pages         = 0x00000020,
    extended_parameter_ec_code                  = 0x00000040,
    commit                                      = 0x00001000,
    reserve                                     = 0x00002000,
    replace_placeholder                         = 0x00004000,
    reserve_placeholder                         = 0x00040000,
    reset                                       = 0x00080000,
    top_down                                    = 0x00100000,
    write_watch                                 = 0x00200000,
    physical                                    = 0x00400000,
    rotate                                      = 0x00800000,
    different_image_base_ok                     = 0x00800000,
    reset_undo                                  = 0x01000000,
    large_pages                                 = 0x20000000,
    pages_4mb                                   = 0x80000000,
    pages_64k                                   = large_pages | physical,
    unmap_with_transient_boost                  = 0x00000001,
    coalesce_placeholders                       = 0x00000001,
    preserve_placeholder                        = 0x00000002,
    decommit                                    = 0x00004000,
    release                                     = 0x00008000,
    free                                        = 0x00010000,
};
ZEN_ENUM_OPERATORS(allocation_type);

enum struct page_protection : u32
{
    noaccess                   = 0x01,
    readonly                   = 0x02,
    readwrite                  = 0x04,
    writecopy                  = 0x08,
    execute                    = 0x10,
    execute_read               = 0x20,
    execute_readwrite          = 0x40,
    execute_writecopy          = 0x80,
    guard                      = 0x100,
    nocache                    = 0x200,
    writecombine               = 0x400,
    graphics_noaccess          = 0x0800,
    graphics_readonly          = 0x1000,
    graphics_readwrite         = 0x2000,
    graphics_execute           = 0x4000,
    graphics_execute_read      = 0x8000,
    graphics_execute_readwrite = 0x10000,
    graphics_coherent          = 0x20000,
    graphics_nocache           = 0x40000,
    enclave_thread_control     = 0x80000000,
    revert_to_file_map         = 0x80000000,
    targets_no_update          = 0x40000000,
    targets_invalid            = 0x40000000,
    enclave_unvalidated        = 0x20000000,
    enclave_mask               = 0x10000000,
    enclave_decommit           = (enclave_mask | 0),
    enclave_ss_first           = (enclave_mask | 1),
    enclave_ss_rest            = (enclave_mask | 2),
};
ZEN_ENUM_OPERATORS(page_protection);

enum struct base_access : u32
{
    remove                   = 0x00010000, // #define DELETE ....
    read_control             = 0x00020000,
    write_dac                = 0x00040000,
    write_owner              = 0x00080000,
    synchronize              = 0x00100000,
    standard_rights_required = 0x000F0000,
    standard_rights_read     = read_control,
    standard_rights_write    = read_control,
    standard_rights_execute  = read_control,
    standard_rights_all      = 0x001F0000,
    rights_all               = 0x0000FFFF,
    system_security          = 0x01000000,
    generic_read             = 0x80000000,
    generic_write            = 0x40000000,
    generic_execute          = 0x20000000,
    generic_all              = 0x10000000,
};
ZEN_ENUM_OPERATORS(base_access);

enum struct process_access : u32
{
    terminate                 = 0x0001,
    create_thread             = 0x0002,
    set_sessionid             = 0x0004,
    vm_operation              = 0x0008,
    vm_read                   = 0x0010,
    vm_write                  = 0x0020,
    dup_handle                = 0x0040,
    create_process            = 0x0080,
    set_quota                 = 0x0100,
    set_information           = 0x0200,
    query_information         = 0x0400,
    suspend_resume            = 0x0800,
    query_limited_information = 0x1000,
    set_limited_information   = 0x2000,
    full                      = std::to_underlying(
                                   base_access::standard_rights_required
                                   | base_access::synchronize
                               ) | 0xFFFF,
};
ZEN_ENUM_OPERATORS(process_access);

enum struct thread_access : u32
{
    terminate                  = 0x0001,
    suspend_resume             = 0x0002,
    get_context                = 0x0008,
    set_context                = 0x0010,
    query_information          = 0x0040,
    set_information            = 0x0020,
    set_thread_token           = 0x0080,
    impersonate                = 0x0100,
    direct_impersonation       = 0x0200,
    set_limited_information    = 0x0400,
    query_limited_information  = 0x0800,
    resume                     = 0x1000,
    all                        = std::to_underlying(
                                    base_access::standard_rights_required
                                    | base_access::synchronize
                                ) | 0xFFFF,
};
ZEN_ENUM_OPERATORS(thread_access);
} //namespace zen::win
