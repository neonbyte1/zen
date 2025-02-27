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

#include <zen/platform/rtl/client_id.hpp>

namespace zen::rtl {
enum struct thread_info : u32
{
    basic,                         // ThreadBasicInformation          => THREAD_BASIC_INFORMATION
    times,                         // ThreadTimes                     => KERNEL_USER_TIMES
    priority,                      // ThreadPriority                  => KPRIORITY
    base_priority,                 // ThreadBasePriority              => LONG
    affinity_mask,                 // ThreadAffinityMask              => KAFFINITY
    impersonation_token,           // ThreadImpersonationToken        => HANDLE
    descriptor_table_entry,        // ThreadDescriptorTableEntry      => DESCRIPTOR_TABLE_ENTRY (or WOW64_DESCRIPTOR_TABLE_ENTRY)
    enable_alignment_fault_fixup,  // ThreadEnableAlignmentFaultFixup => BOOLEAN
    event_pair,                    // ThreadEventPair,
    query_set_win32_start_address, // ThreadQuerySetWin32StartAddress => PVOID
    zero_tls_cell,                 // ThreadZeroTlsCell,
    performance_count,             // ThreadPerformanceCount          => LARGE_INTEGER
    am_i_last_thread,              // ThreadAmILastThread             => ULONG
    ideal_processor,               // ThreadIdealProcessor            => ULONG
    priority_boost,                // ThreadPriorityBoost             => ULONG
    set_tls_array_address,         // ThreadSetTlsArrayAddress,
    is_io_pending,                 // ThreadIsIoPending               => ULONG
    hide_from_debugger,            // ThreadHideFromDebugger          => void
    break_on_termination,          // ThreadBreakOnTermination        => ULONG
    switch_legacy_state,           // ThreadSwitchLegacyState,
    is_terminated,                 // ThreadIsTerminated              => ULONG
    last_system_call,              // ThreadLastSystemCall            => THREAD_LAST_SYSCALL_INFORMATION
    io_priority,                   // ThreadIoPriority                => ULONG
    cycle_time,                    // ThreadCycleTime                 => THREAD_CYCLE_TIME_INFORMATION
    page_priority,                 // ThreadPagePriority              => ULONG
    actual_base_priority,          // ThreadActualBasePriority,
    teb,                           // ThreadTebInformation            => THREAD_TEB_INFORMATION (requires THREAD_GET_CONTEXT + THREAD_SET_CONTEXT)
    c_switch_mon,                  // ThreadCSwitchMon,
    c_switch_pmu,                  // ThreadCSwitchPmu,
    wow64_context,                 // ThreadWow64Context              => WOW64_CONTEXT
    group,                         // ThreadGroupInformatio           => GROUP_AFFINITY
    ums,                           // ThreadUmsInformation,
    counter_profiling,             // ThreadCounterProfiling,
    ideal_processor_ex,            // ThreadIdealProcessorEx          => PROCESSOR_NUMBER
    cpu_accounting,                // ThreadCpuAccountingInformation, // since WIN8
    suspend_count,                 // ThreadSuspendCount                                        // since WINBLUE
    heterogeneous_cpu_policy,      // ThreadHeterogeneousCpuPolicy    => KHETERO_CPU_POLICY     // since THRESHOLD
    container_id,                  // ThreadContainerId,
    name,                          // ThreadNameInformation,
    property,                      // ThreadProperty,
    selected_cpu_sets,             // ThreadSelectedCpuSets,
    system_thread,                 // ThreadSystemThreadInformation,
    max_thread_info_class          // MaxThreadInfoClass
};

template<bool X64 = detail::is_64_bit>
struct thread_basic_information
{
    long_t         exit_status{};
    va_t<X64>      teb_base_address{};
    client_id<X64> client_id{};
    va_t<X64>      affinity_mask{};
    long_t         priority{};
    long_t         base_priority{};
};

enum struct create_thread_flags : u32
{
    none                    = 0x00,
    suspended               = 0x01,
    skip_thread_attach      = 0x02,
    hide_from_debugger      = 0x04,
    has_security_descriptor = 0x10,
    access_check_in_target  = 0x20,
    initial_thread          = 0x80,
};
ZEN_ENUM_OPERATORS(create_thread_flags);

using thread_routine_fn = u32(ZEN_WIN32_CC*)(void*);
} //namespace zen::rtl
