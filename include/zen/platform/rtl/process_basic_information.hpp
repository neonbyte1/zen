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

namespace zen::rtl {
enum class process_info : u32
{
    basic,                                      // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
    quota_limits,                               // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
    io_counters,                                // q: IO_COUNTERS
    vm_counters,                                // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
    times,                                      // q: KERNEL_USER_TIMES
    base_priority,                              // s: KPRIORITY
    raise_priority,                             // s: ULONG
    debug_port,                                 // q: HANDLE
    exception_port,                             // s: PROCESS_EXCEPTION_PORT (requires SeTcbPrivilege)
    access_token,                               // s: PROCESS_ACCESS_TOKEN
    ldt,                                        // qs: PROCESS_LDT_INFORMATION // 10
    ldt_size,                                   // s: PROCESS_LDT_SIZE
    default_hard_error_mode,                    // qs: ULONG
    io_port_handlers,                           // (kernel-mode only) // PROCESS_IO_PORT_HANDLER_INFORMATION
    pooled_usage_and_limits,                    // q: POOLED_USAGE_AND_LIMITS
    working_set_watch,                          // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
    user_mode_iopl,                             // qs: ULONG (requires SeTcbPrivilege)
    enable_alignment_fault_fixup,               // s: BOOLEAN
    priority_class,                             // qs: PROCESS_PRIORITY_CLASS
    wx86,                                       // qs: ULONG (requires SeTcbPrivilege) (VdmAllowed)
    handle_count,                               // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
    affinity_mask,                              // (q >WIN7)s: KAFFINITY, qs: GROUP_AFFINITY
    priority_boost,                             // qs: ULONG
    device_map,                                 // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
    session,                                    // q: PROCESS_SESSION_INFORMATION
    foreground,                                 // s: PROCESS_FOREGROUND_BACKGROUND
    wow64,                                      // q: ULONG_PTR
    image_file_name,                            // q: UNICODE_STRING
    luid_device_maps_enabled,                   // q: ULONG
    break_on_termination,                       // qs: ULONG
    debug_object_handle,                        // q: HANDLE // 30
    debug_flags,                                // qs: ULONG
    handle_tracing,                             // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
    io_priority,                                // qs: IO_PRIORITY_HINT
    execute_flags,                              // qs: ULONG
    tls,                                        // PROCESS_TLS_INFORMATION // ProcessResourceManagement
    cookie,                                     // q: ULONG
    image,                                      // q: SECTION_IMAGE_INFORMATION
    cycle_time,                                 // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
    page_priority,                              // qs: PAGE_PRIORITY_INFORMATION
    instrumentation_callback,                   // s: PVOID or PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION // 40
    thread_stack_allocation,                    // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
    working_set_watch_ex,                       // q: PROCESS_WS_WATCH_INFORMATION_EX[]
    image_file_name_win32,                      // q: UNICODE_STRING
    image_file_mapping,                         // q: HANDLE (input)
    affinity_update_mode,                       // qs: PROCESS_AFFINITY_UPDATE_MODE
    memory_allocation_mode,                     // qs: PROCESS_MEMORY_ALLOCATION_MODE
    group,                                      // q: USHORT[]
    token_virtualization_enabled,               // s: ULONG
    console_host_process,                       // qs: ULONG_PTR // ProcessOwnerInformation
    window,                                     // q: PROCESS_WINDOW_INFORMATION // 50
    handle,                                     // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
    mitigation_policy,                          // s: PROCESS_MITIGATION_POLICY_INFORMATION
    dynamic_function_table,
    handle_checking_mode,                       // qs: ULONG; s: 0 disables, otherwise enables
    keep_alive_count,                           // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
    revoke_file_handles,                        // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
    working_set_control,                        // s: PROCESS_WORKING_SET_CONTROL (requires SeDebugPrivilege)
    handle_table,                               // q: ULONG[] // since WINBLUE
    check_stack_extents_mode,                   // qs: ULONG // KPROCESS->CheckStackExtents (CFG)
    command_line,                               // q: UNICODE_STRING // 60
    protection,                                 // q: PS_PROTECTION
    memory_exhaustion,                          // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
    fault,                                      // PROCESS_FAULT_INFORMATION
    telemetry_id,                               // q: PROCESS_TELEMETRY_ID_INFORMATION
    commit_release,                             // PROCESS_COMMIT_RELEASE_INFORMATION
    default_cpu_sets,                           // SYSTEM_CPU_SET_INFORMATION[5]
    allowed_cpu_sets,                           // SYSTEM_CPU_SET_INFORMATION[5]
    subsystem_process,
    job_memory,                                 // q: PROCESS_JOB_MEMORY_INFO
    in_private,                                 // s: void // ETW // since THRESHOLD2 // 70
    raise_um_exception_on_invalid_handle_close, // qs: ULONG; s: 0 disables, otherwise enables
    ium_challenge_response,
    child_process,                              // q: PROCESS_CHILD_PROCESS_INFORMATION
    high_graphics_priority,                     // qs: BOOLEAN (requires SeTcbPrivilege)
    subsystem,                                  // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
    energy_values,                              // q: PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
    power_throttling_state,                     // qs: POWER_THROTTLING_PROCESS_STATE
    reserved3,                                  // ProcessActivityThrottlePolicy // PROCESS_ACTIVITY_THROTTLE_POLICY
    win32k_syscall_filter,                      // q: WIN32K_SYSCALL_FILTER
    disable_system_allowed_cpu_sets,            // 80
    wake,                                       // PROCESS_WAKE_INFORMATION
    energy_tracking_state,                      // PROCESS_ENERGY_TRACKING_STATE
    manage_writes_to_executable_memory,         // MANAGE_WRITES_TO_EXECUTABLE_MEMORY // since REDSTONE3
    capture_trustlet_live_dump,
    telemetry_coverage,
    enclave,
    enable_read_write_vm_logging,               // PROCESS_READWRITEVM_LOGGING_INFORMATION
    uptime,                                     // q: PROCESS_UPTIME_INFORMATION
    image_section,                              // q: HANDLE
    debug_auth,                                 // since REDSTONE4 // 90
    system_resource_management,                 // PROCESS_SYSTEM_RESOURCE_MANAGEMENT
    sequence_number,                            // q: ULONGLONG
    loader_detour,                              // since REDSTONE5
    security_domain,                            // PROCESS_SECURITY_DOMAIN_INFORMATION
    combine_security_domains,                   // PROCESS_COMBINE_SECURITY_DOMAINS_INFORMATION
    enable_logging,                             // PROCESS_LOGGING_INFORMATION
    leap_second,                                // PROCESS_LEAP_SECOND_INFORMATION
    fiber_shadow_stack_allocation,              // PROCESS_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION // since 19H1
    free_fiber_shadow_stack_allocation,         // PROCESS_FREE_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
    alt_system_call,                            // since 20H1 // 100
    dynamic_eh_continuation_targets,            // PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION
    dynamic_enforced_cet_compatible_ranges,     // PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE_INFORMATION // since 20H2
    create_state_change,                        // since WIN11
    apply_state_change,
    enable_optional_x_state_features,
    alt_prefetch_param,                         // since 22H1
    assign_cpu_partitions,
    priority_class_ex,                          // s: PROCESS_PRIORITY_CLASS_EX
    membership,                                 // PROCESS_MEMBERSHIP_INFORMATION
    effective_io_priority,                      // q: IO_PRIORITY_HINT
    effective_page_priority,                    // q: ULONG
    last_index,
};
template<bool X64 = true>
struct process_basic_information
{
    long      exit_status{};
    va_t<X64> peb{};
    va_t<X64> affinity_mask{};
    long      base_priority{};
    va_t<X64> unique_process_id{};
    va_t<X64> inherited_from_unique_process_id{};
};
} //namespace zen::rtl
