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

#include <zen/platform/rtl/string.hpp>
#include <zen/platform/rtl/large_integer.hpp>
#include <zen/platform/rtl/io_counters.hpp>
#include <zen/platform/rtl/vm_counters.hpp>
#include <zen/platform/rtl/system_thread_information.hpp>

namespace zen::rtl {
enum struct system_info : u32
{
    basic,                                                // q: SYSTEM_BASIC_INFORMATION
    processor,                                            // q: SYSTEM_PROCESSOR_INFORMATION
    performance,                                          // q: SYSTEM_PERFORMANCE_INFORMATION
    time_of_day,                                          // q: SYSTEM_TIMEOFDAY_INFORMATION
    path,                                                 // not implemented
    process,                                              // q: SYSTEM_PROCESS_INFORMATION
    call_count,                                           // q: SYSTEM_CALL_COUNT_INFORMATION
    device,                                               // q: SYSTEM_DEVICE_INFORMATION
    processor_performance,                                // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION (EX in: USHORT ProcessorGroup)
    flags,                                                // q: SYSTEM_FLAGS_INFORMATION
    call_time,                                            // not implemented // SYSTEM_CALL_TIME_INFORMATION // 10
    module,                                               // q: RTL_PROCESS_MODULES
    locks,                                                // q: RTL_PROCESS_LOCKS
    stack_trace,                                          // q: RTL_PROCESS_BACKTRACES
    paged_pool,                                           // not implemented
    non_paged_pool,                                       // not implemented
    handle,                                               // q: SYSTEM_HANDLE_INFORMATION
    object,                                               // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
    page_file,                                            // q: SYSTEM_PAGEFILE_INFORMATION
    vdm_instemul,                                         // q: SYSTEM_VDM_INSTEMUL_INFO
    vdm_bop,                                              // not implemented // 20
    file_cache,                                           // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
    pool_tag,                                             // q: SYSTEM_POOLTAG_INFORMATION
    interrupt,                                            // q: SYSTEM_INTERRUPT_INFORMATION (EX in: USHORT ProcessorGroup)
    dpc_behavior,                                         // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
    full_memory,                                          // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
    load_gdi_driver,                                      // s (kernel-mode only)
    unload_gdi_driver,                                    // s (kernel-mode only)
    time_adjustment,                                      // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
    summary_memory,                                       // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
    mirror_memory,                                        // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
    performance_trace,                                    // q; s: (type depends on EVENT_TRACE_INFORMATION_CLASS)
    obsolete0,                                            // not implemented
    exception,                                            // q: SYSTEM_EXCEPTION_INFORMATION
    crash_dump_state,                                     // s: SYSTEM_CRASH_DUMP_STATE_INFORMATION (requires SeDebugPrivilege)
    kernel_debugger,                                      // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
    context_switch,                                       // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
    registry_quota,                                       // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
    extend_service_table,                                 // s (requires SeLoadDriverPrivilege) // loads win32k only
    priority_seperation,                                  // s (requires SeTcbPrivilege)
    verifier_add_driver,                                  // s (requires SeDebugPrivilege) // 40
    verifier_remove_driver,                               // s (requires SeDebugPrivilege)
    processor_idle,                                       // q: SYSTEM_PROCESSOR_IDLE_INFORMATION (EX in: USHORT ProcessorGroup)
    legacy_driver,                                        // q: SYSTEM_LEGACY_DRIVER_INFORMATION
    current_time_zone,                                    // q; s: RTL_TIME_ZONE_INFORMATION
    lookaside,                                            // q: SYSTEM_LOOKASIDE_INFORMATION
    time_slip_notification,                               // s: HANDLE (NtCreateEvent) (requires SeSystemtimePrivilege)
    session_create,                                       // not implemented
    session_detach,                                       // not implemented
    session,                                              // not implemented (SYSTEM_SESSION_INFORMATION)
    range_start,                                          // q: SYSTEM_RANGE_START_INFORMATION // 50
    verifier,                                             // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
    verifier_thunk_extend,                                // s (kernel-mode only)
    session_process,                                      // q: SYSTEM_SESSION_PROCESS_INFORMATION
    load_gdi_driver_in_system_space,                      // s: SYSTEM_GDI_DRIVER_INFORMATION (kernel-mode only) (same as SystemLoadGdiDriverInformation)
    numa_processor_map,                                   // q: SYSTEM_NUMA_INFORMATION
    prefetcher,                                           // q; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
    extended_process,                                     // q: SYSTEM_PROCESS_INFORMATION
    recommended_shared_data_alignment,                    // q: ULONG // KeGetRecommendedSharedDataAlignment
    com_plus_package,                                     // q; s: ULONG
    numa_available_memory,                                // q: SYSTEM_NUMA_INFORMATION // 60
    processor_power,                                      // q: SYSTEM_PROCESSOR_POWER_INFORMATION (EX in: USHORT ProcessorGroup)
    emulation_basic,                                      // q: SYSTEM_BASIC_INFORMATION
    emulation_processor,                                  // q: SYSTEM_PROCESSOR_INFORMATION
    extended_handle,                                      // q: SYSTEM_HANDLE_INFORMATION_EX
    lost_delayed_write,                                   // q: ULONG
    big_pool,                                             // q: SYSTEM_BIGPOOL_INFORMATION
    session_pool_tag,                                     // q: SYSTEM_SESSION_POOLTAG_INFORMATION
    session_mapped_view,                                  // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
    hotpatch,                                             // q; s: SYSTEM_HOTPATCH_CODE_INFORMATION
    object_security_mode,                                 // q: ULONG // 70
    watchdog_timer_handler,                               // s: SYSTEM_WATCHDOG_HANDLER_INFORMATION // (kernel-mode only)
    watchdog_timer,                                       // q: SYSTEM_WATCHDOG_TIMER_INFORMATION // (kernel-mode only)
    logical_processor,                                    // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION (EX in: USHORT ProcessorGroup)
    wow64_shared_information_obsolete,                    // not implemented
    register_firmware_table_information_handler,          // s: SYSTEM_FIRMWARE_TABLE_HANDLER // (kernel-mode only)
    firmware_table,                                       // SYSTEM_FIRMWARE_TABLE_INFORMATION
    module_ex,                                            // q: RTL_PROCESS_MODULE_INFORMATION_EX
    verifier_triage,                                      // not implemented
    superfetch,                                           // q; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
    memory_list,                                          // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
    file_cache_information_ex,                            // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
    thread_priority_client_id,                            // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege)
    processor_idle_cycle_time,                            // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[] (EX in: USHORT ProcessorGroup)
    verifier_cancellation,                                // SYSTEM_VERIFIER_CANCELLATION_INFORMATION // name:wow64:whNT32QuerySystemVerifierCancellationInformation
    processor_power_information_ex,                       // not implemented
    ref_trace,                                            // q; s: SYSTEM_REF_TRACE_INFORMATION // ObQueryRefTraceInformation
    special_pool,                                         // q; s: SYSTEM_SPECIAL_POOL_INFORMATION (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
    process_id,                                           // q: SYSTEM_PROCESS_ID_INFORMATION
    error_port,                                           // s (requires SeTcbPrivilege)
    boot_environment,                                     // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
    hypervisor,                                           // q: SYSTEM_HYPERVISOR_QUERY_INFORMATION
    verifier_information_ex,                              // q; s: SYSTEM_VERIFIER_INFORMATION_EX
    time_zone,                                            // q; s: RTL_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
    image_file_execution_options,                         // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
    coverage,                                             // q: COVERAGE_MODULES s: COVERAGE_MODULE_REQUEST // ExpCovQueryInformation (requires SeDebugPrivilege)
    prefetch_patch,                                       // SYSTEM_PREFETCH_PATCH_INFORMATION
    verifier_faults,                                      // s: SYSTEM_VERIFIER_FAULTS_INFORMATION (requires SeDebugPrivilege)
    system_partition,                                     // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
    system_disk,                                          // q: SYSTEM_SYSTEM_DISK_INFORMATION
    processor_performance_distribution,                   // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION (EX in: USHORT ProcessorGroup) // 100
    numa_proximity_node,                                  // q; s: SYSTEM_NUMA_PROXIMITY_MAP
    dynamic_time_zone,                                    // q; s: RTL_DYNAMIC_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
    code_integrity,                                       // q: SYSTEM_CODEINTEGRITY_INFORMATION // SeCodeIntegrityQueryInformation
    processor_microcode_update,                           // s: SYSTEM_PROCESSOR_MICROCODE_UPDATE_INFORMATION
    processor_brand_string,                               // q: CHAR[] // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
    virtual_address,                                      // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
    logical_processor_and_group,                          // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX (EX in: LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType) // since WIN7 // KeQueryLogicalProcessorRelationship
    processor_cycle_time,                                 // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[] (EX in: USHORT ProcessorGroup)
    store,                                                // q; s: SYSTEM_STORE_INFORMATION (requires SeProfileSingleProcessPrivilege) // SmQueryStoreInformation
    registry_append_string,                               // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
    ait_sampling_value,                                   // s: ULONG (requires SeProfileSingleProcessPrivilege)
    vhd_boot,                                             // q: SYSTEM_VHD_BOOT_INFORMATION
    cpu_quota,                                            // q; s: PS_CPU_QUOTA_QUERY_INFORMATION
    native_basic,                                         // q: SYSTEM_BASIC_INFORMATION
    error_port_timeouts,                                  // SYSTEM_ERROR_PORT_TIMEOUTS
    low_priority_io,                                      // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
    tpm_boot_entropy,                                     // q: TPM_BOOT_ENTROPY_NT_RESULT // ExQueryTpmBootEntropyInformation
    verifier_counters,                                    // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
    paged_pool_information_ex,                            // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
    system_ptes_information_ex,                           // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
    node_distance,                                        // q: USHORT[4*NumaNodes] // (EX in: USHORT NodeNumber)
    acpi_audit,                                           // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
    basic_performance,                                    // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
    query_performance_counter,                            // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
    session_big_pool,                                     // q: SYSTEM_SESSION_POOLTAG_INFORMATION // since WIN8
    boot_graphics,                                        // q; s: SYSTEM_BOOT_GRAPHICS_INFORMATION (kernel-mode only)
    scrub_physical_memory,                                // q; s: MEMORY_SCRUB_INFORMATION
    bad_page,
    processor_profile_control_area,                       // q; s: SYSTEM_PROCESSOR_PROFILE_CONTROL_AREA
    combine_physical_memory,                              // s: MEMORY_COMBINE, MEMORY_COMBINE_INFORMATION_EX, MEMORY_COMBINE_INFORMATION_EX2 // 130
    entropy_interrupt_timing,                             // q; s: SYSTEM_ENTROPY_TIMING_INFORMATION
    console,                                              // q; s: SYSTEM_CONSOLE_INFORMATION
    platform_binary,                                      // q: SYSTEM_PLATFORM_BINARY_INFORMATION (requires SeTcbPrivilege)
    policy,                                               // q: SYSTEM_POLICY_INFORMATION (Warbird/Encrypt/Decrypt/Execute)
    hypervisor_processor_count,                           // q: SYSTEM_HYPERVISOR_PROCESSOR_COUNT_INFORMATION
    device_data,                                          // q: SYSTEM_DEVICE_DATA_INFORMATION
    device_data_enumeration,                              // q: SYSTEM_DEVICE_DATA_INFORMATION
    memory_topology,                                      // q: SYSTEM_MEMORY_TOPOLOGY_INFORMATION
    memory_channel,                                       // q: SYSTEM_MEMORY_CHANNEL_INFORMATION
    boot_logo,                                            // q: SYSTEM_BOOT_LOGO_INFORMATION // 140
    processor_performance_information_ex,                 // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // (EX in: USHORT ProcessorGroup) // since WINBLUE
    critical_process_error_log,
    secure_boot_policy,                                   // q: SYSTEM_SECUREBOOT_POLICY_INFORMATION
    page_file_information_ex,                             // q: SYSTEM_PAGEFILE_INFORMATION_EX
    secure_boot,                                          // q: SYSTEM_SECUREBOOT_INFORMATION
    entropy_interrupt_timing_raw,
    portable_workspace_efi_launcher,                      // q: SYSTEM_PORTABLE_WORKSPACE_EFI_LAUNCHER_INFORMATION
    full_process,                                         // q: SYSTEM_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
    kernel_debugger_information_ex,                       // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
    boot_metadata,                                        // 150
    soft_reboot,                                          // q: ULONG
    elam_certificate,                                     // s: SYSTEM_ELAM_CERTIFICATE_INFORMATION
    offline_dump_config,                                  // q: OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2
    processor_features,                                   // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
    registry_reconciliation,                              // s: NULL (requires admin) (flushes registry hives)
    edid,                                                 // q: SYSTEM_EDID_INFORMATION
    manufacturing,                                        // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
    energy_estimation_config,                             // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
    hypervisor_detail,                                    // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
    processor_cycle_stats,                                // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION (EX in: USHORT ProcessorGroup) // 160
    vm_generation_count,
    trusted_platform_module,                              // q: SYSTEM_TPM_INFORMATION
    kernel_debugger_flags,                                // SYSTEM_KERNEL_DEBUGGER_FLAGS
    code_integrity_policy,                                // q; s: SYSTEM_CODEINTEGRITYPOLICY_INFORMATION
    isolated_user_mode,                                   // q: SYSTEM_ISOLATED_USER_MODE_INFORMATION
    hardware_security_test_interface_results,
    single_module,                                        // q: SYSTEM_SINGLE_MODULE_INFORMATION
    allowed_cpu_sets,                                     // s: SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION
    vsm_protection,                                       // q: SYSTEM_VSM_PROTECTION_INFORMATION (previously SystemDmaProtectionInformation)
    interrupt_cpu_sets,                                   // q: SYSTEM_INTERRUPT_CPU_SET_INFORMATION // 170
    secure_boot_policy_full,                              // q: SYSTEM_SECUREBOOT_POLICY_FULL_INFORMATION
    code_integrity_policy_full,
    affinitized_interrupt_processor,                      // (requires SeIncreaseBasePriorityPrivilege)
    root_silo,                                            // q: SYSTEM_ROOT_SILO_INFORMATION
    cpu_set,                                              // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
    cpu_set_tag,                                          // q: SYSTEM_CPU_SET_TAG_INFORMATION
    win32_wer_start_callout,
    secure_kernel_profile,                                // q: SYSTEM_SECURE_KERNEL_HYPERGUARD_PROFILE_INFORMATION
    code_integrity_platform_manifest,                     // q: SYSTEM_SECUREBOOT_PLATFORM_MANIFEST_INFORMATION // since REDSTONE
    interrupt_steering,                                   // q: in: SYSTEM_INTERRUPT_STEERING_INFORMATION_INPUT, out: SYSTEM_INTERRUPT_STEERING_INFORMATION_OUTPUT // NtQuerySystemInformationEx // 180
    supported_processor_architectures,                    // p: in opt: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] // NtQuerySystemInformationEx
    memory_usage,                                         // q: SYSTEM_MEMORY_USAGE_INFORMATION
    code_integrity_certificate,                           // q: SYSTEM_CODEINTEGRITY_CERTIFICATE_INFORMATION
    physical_memory,                                      // q: SYSTEM_PHYSICAL_MEMORY_INFORMATION // since REDSTONE2
    control_flow_transition,                              // (Warbird/Encrypt/Decrypt/Execute)
    kernel_debugging_allowed,                             // s: ULONG
    activity_moderation_exe_state,                        // SYSTEM_ACTIVITY_MODERATION_EXE_STATE
    activity_moderation_user_settings,                    // SYSTEM_ACTIVITY_MODERATION_USER_SETTINGS
    code_integrity_policies_full,
    code_integrity_unlock,                                // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION // 190
    integrity_quota,
    flush,                                                // q: SYSTEM_FLUSH_INFORMATION
    processor_idle_mask,                                  // q: ULONG_PTR[ActiveGroupCount] // since REDSTONE3
    secure_dump_encryption,
    write_constraint,                                     // SYSTEM_WRITE_CONSTRAINT_INFORMATION
    kernel_va_shadow,                                     // SYSTEM_KERNEL_VA_SHADOW_INFORMATION
    hypervisor_shared_page,                               // SYSTEM_HYPERVISOR_SHARED_PAGE_INFORMATION // since REDSTONE4
    firmware_boot_performance,
    code_integrity_verification,                          // SYSTEM_CODEINTEGRITYVERIFICATION_INFORMATION
    firmware_partition,                                   // SYSTEM_FIRMWARE_PARTITION_INFORMATION // 200
    speculation_control,                                  // SYSTEM_SPECULATION_CONTROL_INFORMATION // (CVE-2017-5715) REDSTONE3 and above.
    dma_guard_policy,                                     // SYSTEM_DMA_GUARD_POLICY_INFORMATION
    enclave_launch_control,                               // SYSTEM_ENCLAVE_LAUNCH_CONTROL_INFORMATION
    workload_allowed_cpu_sets,                            // SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION // since REDSTONE5
    code_integrity_unlock_mode,                           // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION
    leap_second,                                          // SYSTEM_LEAP_SECOND_INFORMATION
    flags2,                                               // q: SYSTEM_FLAGS_INFORMATION
    security_model,                                       // SYSTEM_SECURITY_MODEL_INFORMATION // since 19H1
    code_integrity_synthetic_cache,
    feature_configuration,                                // SYSTEM_FEATURE_CONFIGURATION_INFORMATION // since 20H1 // 210
    feature_configuration_section,                        // SYSTEM_FEATURE_CONFIGURATION_SECTIONS_INFORMATION
    feature_usage_subscription,                           // SYSTEM_FEATURE_USAGE_SUBSCRIPTION_DETAILS
    secure_speculation_control,                           // SECURE_SPECULATION_CONTROL_INFORMATION
    spaces_boot,                                          // since 20H2
    fw_ramdisk,                                           // SYSTEM_FIRMWARE_RAMDISK_INFORMATION
    whea_ipmi_hardware,
    dif_set_rule_class,                                   // SYSTEM_DIF_VOLATILE_INFORMATION
    dif_clear_rule_class,
    dif_apply_plugin_verification_on_driver,              // SYSTEM_DIF_PLUGIN_DRIVER_INFORMATION
    dif_remove_plugin_verification_on_driver,             // SYSTEM_DIF_PLUGIN_DRIVER_INFORMATION // 220
    shadow_stack,                                         // SYSTEM_SHADOW_STACK_INFORMATION
    build_version,                                        // q: in: ULONG (LayerNumber), out: SYSTEM_BUILD_VERSION_INFORMATION // NtQuerySystemInformationEx // 222
    pool_limit,                                           // SYSTEM_POOL_LIMIT_INFORMATION (requires SeIncreaseQuotaPrivilege)
    code_integrity_add_dynamic_store,
    code_integrity_clear_dynamic_stores,
    dif_pool_tracking,
    pool_zeroing,                                         // q: SYSTEM_POOL_ZEROING_INFORMATION
    dpc_watchdog,                                         // q; s: SYSTEM_DPC_WATCHDOG_CONFIGURATION_INFORMATION
    dpc_watchdog_information2,                            // q; s: SYSTEM_DPC_WATCHDOG_CONFIGURATION_INFORMATION_V2
    supported_processor_architectures2,                   // q: in opt: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] // NtQuerySystemInformationEx // 230
    single_processor_relationship,                        // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // (EX in: PROCESSOR_NUMBER Processor)
    xfg_check_failure,                                    // q: SYSTEM_XFG_FAILURE_INFORMATION
    iommu_state,                                          // SYSTEM_IOMMU_STATE_INFORMATION // since 22H1
    hypervisor_minroot,                                   // SYSTEM_HYPERVISOR_MINROOT_INFORMATION
    hypervisor_boot_pages,                                // SYSTEM_HYPERVISOR_BOOT_PAGES_INFORMATION
    pointer_auth,                                         // SYSTEM_POINTER_AUTH_INFORMATION
    secure_kernel_debugger,
    original_image_feature,                               // q: in: SYSTEM_ORIGINAL_IMAGE_FEATURE_INFORMATION_INPUT, out: SYSTEM_ORIGINAL_IMAGE_FEATURE_INFORMATION_OUTPUT // NtQuerySystemInformationEx
    last_index
};

template<bool X64 = true>
struct system_process_information
{
    u32                            next_entry_delta{};
    u32                            num_threads{};
    u32                            reserved1[6]{};
    large_integer                  create_time{};
    large_integer                  user_time{};
    large_integer                  kernel_time{};
    unicode_string<X64>            image_name{};
    u32                            base_priority{};
    va_t<X64>                      unique_process_id{};
    va_t<X64>                      inherited_from_unique_process_id{};
    u32                            num_handles{};
    u32                            session_id{};
    va_t<X64>                      unique_process_key{};
    vm_counters<X64>               vm_counters{};
    va_t<X64>                      private_page_count{};
    rtl::io_counters               io_counters{};
    system_thread_information<X64> threads[1]{};

    NODISCARD
    constexpr
    auto
    pid() const noexcept -> u32
    {
        return static_cast<u32>(unique_process_id);
    }

    NODISCARD
    auto
    next() noexcept -> system_process_information*
    {
        return reinterpret_cast<system_process_information*>(
            reinterpret_cast<u8*>(this) + next_entry_delta
        );
    }

    NODISCARD
    auto
    next() const noexcept -> const system_process_information*
    {
        return const_cast<system_process_information*>(this)->next();
    }
};

template<bool X64 = detail::is_64_bit>
struct system_module
{
    va_t<X64> section{};
    va_t<X64> mapped_base{};
    va_t<X64> image_base{};
    u32       size{};
    u32       flags{};
    u16       index{};
    u16       rank{};
    u16       load_count{};
    u16       module_name_offset{};
    char      image_name[256]{};
};

template<bool X64 = detail::is_64_bit>
struct system_module_information
{
    u32                count{};
    system_module<X64> modules[1]{};
};
} //namespace zen::rtl
