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
#include <zen/platform/heavensgate.hpp>
#if defined(ZEN_TARGET_32_BIT)
#include <zen/platform/rtl/peb.hpp>
#include <zen/nt/directories/exports.hpp>
#include <zen/nt/image.hpp>
#include <zen/core/fnv.hpp>
#include <cstdarg>

using namespace zen;

#define ZEN_DECL_HEAVENSGATE_METADATA(ModuleName, FunctionName)                                            \
    static u64 function_address;                                                                           \
    if (!function_address) {                                                                               \
        constexpr static auto module_hash = zen::fnv<>::hash<true>(ModuleName);                            \
        constexpr static auto name_hash = zen::fnv<>::hash<true>(FunctionName);                            \
        function_address = win::x64_get_proc_address(win::x64_get_module_handle(module_hash), name_hash);  \
        if (!function_address) {                                                                           \
            return /*STATUS_INVALID_SYSTEM_SERVICE*/ static_cast<long>(0xC000001C);                        \
        }                                                                                                  \
    }

namespace {
NODISCARD
auto
x64_get_teb() noexcept -> u64
{
    auto reg_rax = win::make_reg64(0);

    __asm {
        X64_Start();
        X64_Push(X64_R12);
        pop reg_rax.dw[0]
        X64_End();
    }

    return reg_rax.u64;
}

NODISCARD
auto
x64_get_proc_address(
    const u64  handle,
    const u32  proc_hash,
    const i32  proc_ordinal,
    const bool lowercase
) noexcept -> u64
{
    win::dos_header       dos_hdr{};
    win::nt_headers<true> nt_hdr{};
    win::export_directory exp_dir{};

    win::x64_memcpy(&dos_hdr, handle, sizeof(dos_hdr));

    if (!dos_hdr.valid()) {
        return 0;
    }

    win::x64_memcpy(&nt_hdr, handle + dos_hdr.next_hdr_offset(), sizeof(nt_hdr));

    if (!nt_hdr.valid() || !nt_hdr.is_64_bit()) {
        return 0;
    }

    const auto& data_dir = nt_hdr.optional_hdr().data_directories().export_directory;

    if (!data_dir.present()) {
        return 0;
    }

    const auto exp_dir_ptr = handle + data_dir.rva();
    const auto delta       = exp_dir_ptr - data_dir.rva();

    win::x64_memcpy(&exp_dir, exp_dir_ptr, sizeof(exp_dir));

    const auto num_functions = exp_dir.num_functions();
    const auto num_names     = exp_dir.num_names();

    if (!num_functions) {
        return 0;
    }

    auto names     = std::make_unique<u32[]>(num_names);
    auto functions = std::make_unique<u32[]>(num_functions);
    auto ordinals  = std::make_unique<u16[]>(num_functions);

    win::x64_memcpy(
        ordinals.get(),
        handle + exp_dir.rva_name_ordinals(),
        sizeof(u16) * exp_dir.num_functions()
    );
    win::x64_memcpy(
        functions.get(),
        handle + exp_dir.rva_functions(),
        sizeof(u32) * exp_dir.num_functions()
    );
    win::x64_memcpy(
        names.get(),
        handle + exp_dir.rva_names(),
        sizeof(u32) * exp_dir.num_names()
    );

    for (u32 i{}; i < num_functions; ++i) {
        const auto ordinal = proc_ordinal != -1
            ? static_cast<u16>(i)
            : ordinals[i];

        const auto found_by_ordinal
            = proc_ordinal != -1
            && static_cast<u16>(proc_ordinal) == exp_dir.base() + ordinal;

        bool found_by_name{};

        if (!found_by_ordinal && i < num_names) {
            char buffer[260]{};
            win::x64_memcpy(buffer, delta + names[i], sizeof(buffer));

            const std::string_view function_name{buffer};

            const auto hash = lowercase
                ? fnv<>::get<true>(function_name)
                : fnv<>::get(function_name);

            found_by_name = hash == proc_hash;
        }

        if (!found_by_ordinal && !found_by_name) {
            continue;
        }

        const auto function_pointer = delta + functions[ordinal];
        const auto directory_begin  = delta + data_dir.rva();
        const auto directory_end    = directory_begin + data_dir.size();

        if (function_pointer >= directory_begin && function_pointer <= directory_end) {
            // e.g. "api-ms-win-core-processthreads-l1-1-6.SetProcessDynamicEnforcedCetCompatibleRanges"
            char forwarded_name_raw[260]{};
            win::x64_memcpy(forwarded_name_raw, function_pointer, sizeof(forwarded_name_raw));
            const std::string_view forwarded_name{forwarded_name_raw};
            const auto             dot_pos{forwarded_name.find('.')};

            if (dot_pos == std::string_view::npos) {
                return 0;
            }

            // e.g. "api-ms-win-core-processthreads-l1-1-6" + ".dll"
            const auto lib_hash         = fnv<>::hash(".dll", fnv<>::get<true>(forwarded_name.substr(0, dot_pos)));
            const auto forwarded_handle = win::x64_get_module_handle(lib_hash);

            if (!forwarded_handle) {
                return 0;
            }

            return forwarded_handle
                ? x64_get_proc_address(
                    forwarded_handle,
                    // e.g. "SetProcessDynamicEnforcedCetCompatibleRanges"
                    fnv<>::get<true>(forwarded_name.substr(dot_pos + 1)),
                    -1,
                    true
                ) : 0;
        }

        return function_pointer;
    }

    return 0;
}

NODISCARD
auto
x64_to_handle(
    const u64 handle
) noexcept -> void*
{
    return reinterpret_cast<void*>(static_cast<iptr>(handle));
}

NODISCARD
auto
x64_nt_open_process(
    const u32    pid,
    const u32    desired_access,
    void** const process_handle
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtOpenProcess")

    auto                         handle{win::invalid_handle_value<u64>()};
    rtl::object_attributes<true> attributes{};
    rtl::client_id<true>         cid{};

    cid.unique_process = pid;

    const auto status = win::x64_call(
        function_address,
        4,
        win::x64_from_ptr(&handle),
        static_cast<u64>(desired_access),
        win::x64_from_ptr(&attributes),
        win::x64_from_ptr(&cid)
    );

    *process_handle = x64_to_handle(handle);

    return static_cast<long_t>(status);
}

auto
x64_nt_create_thread_ex(
    void**                              output,
    const void* const                   process_handle,
    const u64                           routine,
    const u64                           thread_params,
    const rtl::create_thread_flags      flags,
    const u32                           desired_access,
    rtl::object_attributes<true>* const attributes,
    const u64                           stack_zero_bits,
    const u64                           size_of_stack_commit,
    const u64                           size_of_stack_reserve,
    void* const                         bytes_buffer
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtCreateThreadEx")

    auto       handle{win::invalid_handle_value<u64>()};
    const auto status = win::x64_call(
        0,
        11,
        win::x64_from_ptr(&handle),
        static_cast<u64>(desired_access),
        win::x64_from_ptr(attributes),
        win::x64_from_handle(process_handle),
        routine,
        thread_params,
        static_cast<u64>(flags),
        stack_zero_bits,
        size_of_stack_commit,
        size_of_stack_reserve,
        win::x64_from_handle(bytes_buffer)
    );

    *output = x64_to_handle(handle);

    return static_cast<long_t>(status);
}
}

auto
win::x64_get_peb() noexcept -> u64
{
    // https://en.wikipedia.org/wiki/Win32_Thread_Information_Block#Contents_of_the_TIB_on_Windows
    // Type     Offset (x86)    Offset (x64)    Description
    // pointer 	FS:[0x30] 	    GS:[0x60] 	    Linear address of Process Environment Block (PEB)

    u64 peb_addr{};
    win::x64_memcpy(&peb_addr, x64_get_teb() + 0x60, sizeof(peb_addr));

    return peb_addr;
}

auto
win::x64_get_pid() noexcept -> u32
{
    // https://en.wikipedia.org/wiki/Win32_Thread_Information_Block#Contents_of_the_TIB_on_Windows
    // Type     Offset (x86)    Offset (x64)    Description
    // pointer 	FS:[0x20] 	    GS:[0x40] 	    Process ID (in some Windows distributions this field is used as DebugContext)

    u64 pid{};
    win::x64_memcpy(&pid, x64_get_teb() + 0x40, sizeof(pid));

    return static_cast<u32>(pid);
}

#pragma warning(push)
#pragma warning(disable : 4409)
auto
win::x64_call(
    u64 function,
    szt num_arguments,
    ...
) noexcept -> u64
{
    va_list args;
    va_start(args, num_arguments);

    auto arg_reg = [&num_arguments, &args]
    {
        if (num_arguments > 0) {
            --num_arguments;

            return make_reg64(va_arg(args, u64));
        }

        return make_reg64(0);
    };

    auto reg_rcx   = arg_reg();
    auto reg_rdx   = arg_reg();
    auto reg_r8    = arg_reg();
    auto reg_r9    = arg_reg();
    auto reg_rax   = make_reg64(0);
    auto rest_args = make_reg64(x64_from_ptr(&va_arg(args, u64)));
    auto num_args  = make_reg64(num_arguments);
    u32  back_esp{};
    u16  back_fs{};

    __asm {
        ;// reset FS segment, to properly handle RFG
            mov    back_fs, fs
            mov    eax, 0x2B
            mov    fs, ax

        ;// keep original esp in back_esp variable
            mov    back_esp, esp

        ;// align esp to 0x10, without aligned stack some syscalls may return errors !
        ;// (actually, for syscalls it is sufficient to align to 8, but SSE opcodes
        ;// requires 0x10 alignment), it will be further adjusted according to the
        ;// number of arguments above 4
            and    esp, 0xFFFFFFF0

            X64_Start();

        ;// below code is compiled as x86 inline asm, but it is executed as x64 code
        ;// that's why it need sometimes REX_W() macro, right column contains detailed
        ;// transcription how it will be interpreted by CPU

        ;// fill first four arguments
X64_REX_W   mov    ecx, reg_rcx.dw[0]                       ;// mov     rcx, qword ptr [reg_rcx]
X64_REX_W   mov    edx, reg_rdx.dw[0]                       ;// mov     rdx, qword ptr [reg_rdx]
            push   reg_r8.u64                               ;// push    qword ptr [reg_r8]
            X64_Pop(X64_R8);                                ;// pop     r8
            push   reg_r9.u64                               ;// push    qword ptr [reg_r9]
            X64_Pop(X64_R9);                                ;// pop     r9
X64_REX_W   mov    eax, num_args.dw[0]                      ;// mov     rax, qword ptr [num_args]
        ;// final stack adjustment, according to the
        ;// number of arguments above 4
            test   al, 1                                    ;// test    al, 1
            jnz    _no_adjust                               ;// jnz     _no_adjust
            sub    esp, 8                                   ;// sub     rsp, 8
        _no_adjust:
            push   edi                                      ;// push    rdi
X64_REX_W   mov    edi, rest_args.dw[0]                     ;// mov     rdi, qword ptr [rest_args]
        ;// put rest of arguments on the stack
X64_REX_W   test   eax, eax                                 ;// test    rax, rax
            jz     _ls_e                                    ;// je      _ls_e
X64_REX_W   lea    edi, dword ptr [edi + 8*eax - 8]         ;// lea     rdi, [rdi + rax*8 - 8]
        _ls:
X64_REX_W   test   eax, eax                                 ;// test    rax, rax
            jz     _ls_e                                    ;// je      _ls_e
            push   dword ptr [edi]                          ;// push    qword ptr [rdi]
X64_REX_W   sub    edi, 8                                   ;// sub     rdi, 8
X64_REX_W   sub    eax, 1                                   ;// sub     rax, 1
            jmp    _ls                                      ;// jmp     _ls
        _ls_e:
        ;// create stack space for spilling registers
X64_REX_W   sub    esp, 0x20                                ;// sub     rsp, 20h
            call   function                                 ;// call    qword ptr [func]
        ;// cleanup stack
X64_REX_W   mov    ecx, num_args.dw[0]                      ;// mov     rcx, qword ptr [num_args]
X64_REX_W   lea    esp, dword ptr [esp + 8*ecx + 0x20]      ;// lea     rsp, [rsp + rcx*8 + 20h]
            pop    edi                                      ;// pop     rdi
        ;// set return value
X64_REX_W   mov    reg_rax.dw[0], eax                       ;// mov     qword ptr [reg_rax], rax
            X64_End();
            mov    ax, ds
            mov    ss, ax
            mov    esp, back_esp
        ;// restore FS segment
            mov    ax, back_fs
            mov    fs, ax
    }

    return reg_rax.u64;
}
#pragma warning(pop)

auto
win::x64_memcpy(
    void* dst,
    u64   src,
    szt   num_bytes
) noexcept -> void
{
    auto reg_src = make_reg64(src);

    // below code is compiled as x86 inline asm, but it is executed as x64 code
    // that's why it need sometimes REX_W() macro, right column contains detailed
    // transcription how it will be interpreted by CPU
    __asm {
            X64_Start();
            push   edi                  ;// push     rdi
            push   esi                  ;// push     rsi
            mov    edi, dst             ;// mov      edi, dword ptr [dst]      ; high part of RDI is zeroed
X64_REX_W   mov    esi, reg_src.dw[0]   ;// mov      rsi, qword ptr [reg_src]
            mov    ecx, num_bytes       ;// mov      ecx, dword ptr [size]     ; high part of RCX is zeroed
            mov    eax, ecx             ;// mov      eax, ecx
            and eax, 3                  ;// and      eax, 3
            shr    ecx, 2               ;// shr      ecx, 2
            rep    movsd                ;// rep movs dword ptr [rdi], dword ptr [rsi]
            test   eax, eax             ;// test     eax, eax
            je     _move_0              ;// je       _move_0
            cmp    eax, 1               ;// cmp      eax, 1
            je     _move_1              ;// je       _move_1
            movsw                       ;// movs     word ptr [rdi], word ptr [rsi]
            cmp    eax, 2               ;// cmp      eax, 2
            je     _move_0              ;// je       _move_0
        _move_1:
            movsb                       ;// movs     byte ptr [rdi], byte ptr [rsi]
        _move_0:
            pop    esi                  ;// pop      rsi
            pop    edi                  ;// pop      rdi
            X64_End();
    }
}

auto
win::x64_memcmp(
    const void* dst,
    u64         src,
    szt         num_bytes
) noexcept -> bool
{
    bool result{};
    auto reg_src = make_reg64(src);

    // below code is compiled as x86 inline asm, but it is executed as x64 code
    // that's why it need sometimes REX_W() macro, right column contains detailed
    // transcription how it will be interpreted by CPU
    __asm {
            X64_Start();
            push   edi                      ;// push      rdi
            push   esi                      ;// push      rsi
            mov    edi, dst                 ;// mov       edi, dword ptr [dst]       ; high part of RDI is zeroed
X64_REX_W   mov    esi, reg_src.dw[0]       ;// mov       rsi, qword ptr [reg_src]
            mov    ecx, num_bytes           ;// mov       ecx, dword ptr [num_bytes] ; high part of RCX is zeroed
            mov    eax, ecx                 ;// mov       eax, ecx
            and eax, 3                      ;// and       eax, 3
            shr    ecx, 2                   ;// shr       ecx, 2
            repe   cmpsd                    ;// repe cmps dword ptr [rsi], dword ptr [rdi]
            jnz     _ret_false              ;// jnz       _ret_false
            test   eax, eax                 ;// test      eax, eax
            je     _move_0                  ;// je        _move_0
            cmp    eax, 1                   ;// cmp       eax, 1
            je     _move_1                  ;// je        _move_1
            cmpsw                           ;// cmps      word ptr [rsi], word ptr [rdi]
            jnz     _ret_false              ;// jnz       _ret_false
            cmp    eax, 2                   ;// cmp       eax, 2
            je     _move_0                  ;// je        _move_0
        _move_1:
            cmpsb                           ;// cmps      byte ptr [rsi], byte ptr [rdi]
            jnz     _ret_false              ;// jnz       _ret_false
        _move_0:
            mov    result, 1                ;// mov       byte ptr [result], 1
        _ret_false:
            pop    esi                      ;// pop      rsi
            pop    edi                      ;// pop      rdi

            X64_End();
    }

    return result;
}

auto
win::x64_get_module_handle(
    const u32  name,
    const bool lowercase
) noexcept -> u64
{
    const auto validate_entry = [&](
        const rtl::ldr_data_table_entry<true>& entry
    ) noexcept -> bool
    {
        if (!entry.base_dll_name.buffer()) {
            return false;
        }

        if (!fnv<>::valid(name)) {
            return true;
        }

        const auto module_name_buffer
            = std::make_unique<wchar_t[]>(entry.base_dll_name.length() + 1);

        x64_memcpy(
            module_name_buffer.get(),
            entry.base_dll_name.buffer(),
            entry.base_dll_name.len()
        );

        const std::wstring_view module_name{
            module_name_buffer.get(),
            entry.base_dll_name.length()
        };

        const auto hash = lowercase
            ? fnv<>::get<true>(module_name)
            : fnv<>::get(module_name);

        return hash == name;
    };

    rtl::peb<true> peb{};
    win::x64_memcpy(&peb, x64_get_peb(), sizeof(peb));

    if (peb.ldr) {
        rtl::peb_ldr_data<true>         ldr{};
        rtl::ldr_data_table_entry<true> head{};

        x64_memcpy(&ldr, peb.ldr, sizeof(ldr));

        const auto last_entry = peb.ldr
            + offsetof(rtl::peb_ldr_data<true>, in_load_order_module_list);

        head.in_load_order_links.next = ldr.in_load_order_module_list.next;

        do {
            x64_memcpy(&head, head.in_load_order_links.next, sizeof(head));

            if (validate_entry(head)) {
                return head.dll_base;
            }
        } while (head.in_load_order_links.next != last_entry);
    }

    return 0;
}

auto
win::x64_get_module_handle(
    const std::string_view name,
    const bool             lowercase
) noexcept -> u64
{
    const u32 name_hash = !name.empty()
        ? (lowercase ? fnv<>::get<true>(name) : fnv<>::get(name))
        : 0;

    return x64_get_module_handle(name_hash, lowercase);
}

auto
win::x64_get_module_handle(
    const std::wstring_view name,
    const bool              lowercase
) noexcept -> u64
{
    const u32 name_hash = !name.empty()
        ? (lowercase ? fnv<>::get<true>(name) : fnv<>::get(name))
        : 0;

    return x64_get_module_handle(name_hash, lowercase);
}

auto
win::x64_get_proc_address(
    const u64  handle,
    const u32  name,
    const bool lowercase
) noexcept -> u64
{
    if (!handle || !fnv<>::valid(name)) {
        return 0;
    }

    return ::x64_get_proc_address(handle, name, -1, lowercase);
}

auto
win::x64_get_proc_address(
    const u64              handle,
    const std::string_view name,
    const bool             lowercase
) noexcept -> u64
{
    if (!handle || name.empty()) {
        return 0;
    }

    const auto name_ptr   = reinterpret_cast<const uptr>(name.data());
    const auto is_ordinal = name_ptr <= 0xFFFF;

    return ::x64_get_proc_address(
        handle,
        !is_ordinal
            ? (lowercase ? fnv<>::get<true>(name) : fnv<>::get(name))
            : 0,
        is_ordinal ? static_cast<i32>(name_ptr) : -1,
        lowercase
    );
}

auto
win::x64_nt_query_information_process(
    const void* const       process_handle,
    const rtl::process_info id,
    void* const             buffer,
    const u64               size,
    u64* const              return_length
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtQueryInformationProcess")

    u64        ret_len{};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(process_handle),
        static_cast<u64>(id),
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&ret_len)
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
}

auto
win::x64_nt_query_information_thread(
    const void*  const     thread_handle,
    const rtl::thread_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtQueryInformationThread")

    u64        ret_len{};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(thread_handle),
        static_cast<u64>(id),
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&ret_len)
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
}

auto
win::x64_nt_query_system_information(
    const rtl::system_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtQuerySystemInformation")

    u64        ret_len{};
    const auto status = x64_call(
        function_address,
        4,
        static_cast<u64>(id),
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&ret_len)
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
}

auto
win::x64_nt_query_object(
    const void* const      handle,
    const rtl::object_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtQueryObject")

    u64        ret_len{};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(handle),
        static_cast<u64>(id),
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&ret_len)
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
}

auto
win::x64_nt_query_section(
    const void* const       handle,
    const rtl::section_info id,
    void* const             buffer,
    const u64               size,
    u64* const              return_length
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtQuerySection")

    u64        ret_len{};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(handle),
        static_cast<u64>(id),
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&ret_len)
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
}

auto
win::x64_nt_query_virtual_memory(
    const void* const      handle,
    const u64              base_address,
    const rtl::memory_info id,
    void* const            buffer,
    const u64              size,
    u64* const             return_length
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtQueryVirtualMemory")

    u64        ret_len{};
    const auto status = x64_call(
        function_address,
        6,
        x64_from_handle(handle),
        base_address,
        static_cast<u64>(id),
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&ret_len)
    );

    if (return_length) {
        *return_length = ret_len;
    }

    return status;
}

auto
win::x64_nt_allocate_virtual_memory(
    const void* const     handle,
    u64*                  base_address,
    const u64             zero_bits,
    const u64             size,
    const allocation_type allocation,
    const page_protection protection
) noexcept -> long_t
{

    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtAllocateVirtualMemory")

    auto       region_size{size};
    const auto status = x64_call(
        function_address,
        6,
        x64_from_handle(handle),
        x64_from_ptr(base_address),
        zero_bits,
        x64_from_ptr(&region_size),
        static_cast<u64>(allocation),
        static_cast<u64>(protection)
    );

    return status;
}

auto
win::x64_nt_free_virtual_memory(
    const void* const handle,
    const u64         base_address,
    const u64         size,
    const u32         free_type
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtFreeVirtualMemory")

    auto       temp_address{base_address};
    auto       region_size{size};
    const auto status = x64_call(
        function_address,
        4,
        x64_from_handle(handle),
        x64_from_ptr(&temp_address),
        x64_from_ptr(&region_size),
        static_cast<u64>(free_type)
    );

    return status;
}

auto
win::x64_nt_protect_virtual_memory(
    const void* const      handle,
    const u64              base_address,
    const u64              size,
    const page_protection  new_protection,
    page_protection* const old_protection
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtProtectVirtualMemory")

    auto       temp_address{base_address};
    auto       region_size{size};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(handle),
        x64_from_ptr(&temp_address),
        x64_from_ptr(&region_size),
        static_cast<u64>(new_protection),
        x64_from_ptr(old_protection)
    );

    return status;
}

auto
win::x64_nt_read_virtual_memory(
    const void* const handle,
    const u64         base_address,
    void* const       buffer,
    const u64         size,
    u64* const        bytes_read
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtReadVirtualMemory")

    u64        bytes_processed{};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(handle),
        base_address,
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&bytes_processed)
    );

    if (bytes_read) {
        *bytes_read = bytes_processed;
    }

    return status;
}

auto
win::x64_nt_write_virtual_memory(
    const void* const handle,
    const u64         base_address,
    const void* const buffer,
    const u64         size,
    u64* const        bytes_written
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtWriteVirtualMemory")

    u64        bytes_processed{};
    const auto status = x64_call(
        function_address,
        5,
        x64_from_handle(handle),
        base_address,
        x64_from_ptr(buffer),
        size,
        x64_from_ptr(&bytes_processed)
    );

    if (bytes_written) {
        *bytes_written = bytes_processed;
    }

    return status;
}

auto
win::x64_nt_get_context_thread(
    const void* handle,
    void*       context
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtGetContextThread")

    return static_cast<long_t>(
        x64_call(
            function_address,
            2,
            x64_from_handle(handle),
            x64_from_ptr(context)
        )
    );
}

auto
win::x64_nt_set_context_thread(
    const void* handle,
    const void* context
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtSetContextThread")

    return static_cast<long_t>(
        x64_call(
            function_address,
            2,
            x64_from_handle(handle),
            x64_from_ptr(context)
        )
    );
}

auto
win::x64_nt_close(
    const void* const handle
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtClose")

    const auto status = is_valid_handle(handle)
        ? static_cast<long_t>(x64_call(function_address, 1, x64_from_handle(handle)))
        : 0;

    return status;
}

auto
win::x64_nt_open_process(
    const u32            pid,
    const process_access desired_access,
    long_t* const        returnred_status
) noexcept -> void*
{
    void*      handle{};
    const auto status = ::x64_nt_open_process(
        pid,
        std::to_underlying(desired_access),
        &handle
    );

    if (returnred_status) {
        *returnred_status = status;
    }

    return handle;
}

auto
win::x64_nt_suspend_process(
    const void* const process_handle
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtSuspendProcess")

    const auto status = is_valid_handle(process_handle)
        ? static_cast<long_t>(x64_call(function_address, 1, x64_from_handle(process_handle)))
        : 0;

    return status;
}

auto
win::x64_nt_resume_process(
    const void* process_handle
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtResumeProcess")

    const auto status = is_valid_handle(process_handle)
        ? static_cast<long_t>(x64_call(function_address, 1, x64_from_handle(process_handle)))
        : 0;

    return status;
}

auto
win::x64_nt_terminate_process(
    const void* const process_handle,
    const long_t      exit_status
) noexcept -> long_t
{
    if (!is_valid_handle(process_handle)) {
        return 0;
    }

    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtTerminateProcess")

    return static_cast<long_t>(
        x64_call(
            function_address,
            2,
            x64_from_handle(process_handle),
            static_cast<u64>(exit_status)
        )
    );
}

auto
win::x64_nt_create_thread_ex(
    const void* const                   process_handle,
    const u64                           routine,
    const u64                           thread_params,
    const rtl::create_thread_flags      flags,
    const thread_access                 desired_access,
    rtl::object_attributes<true>* const attributes,
    const u64                           stack_zero_bits,
    const u64                           size_of_stack_commit,
    const u64                           size_of_stack_reserve,
    void* const                         bytes_buffer
) noexcept -> void*
{
    void* handle{};

    ::x64_nt_create_thread_ex(
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
}

auto
win::x64_nt_suspend_thread(
    const void* const thread_handle,
    u32* const        previous_suspend_count
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtSuspendThread")

    u32        count{};
    const auto status = is_valid_handle(thread_handle)
        ? x64_call(function_address, 2, x64_from_handle(thread_handle), &count)
        : 0;

    if (previous_suspend_count) {
        *previous_suspend_count = count;
    }

    return status;
}

auto
win::x64_nt_resume_thread(
    const void* const thread_handle,
    u32* const        suspend_count
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtResumeThread")

    return static_cast<long_t>(
        x64_call(
            function_address,
            2,
            x64_from_handle(thread_handle),
            x64_from_ptr(suspend_count)
        )
    );
}

auto
win::x64_nt_wait_for_single_object(
    const void* const handle,
    const bool        alertable,
    i64*  const       time
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtWaitForSingleObject")

    return static_cast<long_t>(
        x64_call(
            function_address,
            3,
            x64_from_handle(handle),
            static_cast<u64>(alertable),
            x64_from_ptr(time)
        )
    );
}

auto
win::x64_nt_create_section(
    void** const                        section_handle,
    const u32                           desired_access,
    rtl::object_attributes<true>* const attributes,
    i64* const                          maximum_size,
    const u32                           page_attributes,
    const u32                           section_attributes,
    void* const                         file_handle
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtCreateSection")

    auto       handle{invalid_handle_value<u64>()};
    const auto status = x64_call(
        function_address,
        10,
        x64_from_ptr(&handle),
        static_cast<u64>(desired_access),
        x64_from_ptr(attributes),
        x64_from_ptr(maximum_size),
        static_cast<u64>(page_attributes),
        static_cast<u64>(section_attributes),
        x64_from_ptr(file_handle)
    );

    *section_handle = x64_to_handle(handle);

    return status;
}

auto
win::x64_nt_map_view_of_section(
    const void* const          section_handle,
    const void* const          process_handle,
    u64* const                 base_address,
    const u64                  zero_bits,
    const u64                  commit_size,
    i64* const                 section_offset,
    u64* const                 view_size,
    const rtl::section_inherit inherit_disposition,
    const allocation_type      allocation,
    const page_protection      protection
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtMapViewOfSection")

    return static_cast<long_t>(
        x64_call(
            function_address,
            10,
            x64_from_handle(section_handle),
            x64_from_handle(process_handle),
            x64_from_ptr(base_address),
            zero_bits,
            commit_size,
            x64_from_ptr(section_offset),
            x64_from_ptr(view_size),
            static_cast<u64>(inherit_disposition),
            static_cast<u64>(allocation),
            static_cast<u64>(protection)
        )
    );
}

auto
win::x64_nt_map_view_of_section(
    const void* const          section_handle,
    const void* const          process_handle,
    u64                        size,
    const allocation_type      allocation,
    const page_protection      protection,
    const rtl::section_inherit inherit_disposition,
    const u64                  zero_bits,
    const u64                  commit_size,
    i64* const                 section_offset
) noexcept -> u64
{
    u64 base_address{};

    x64_nt_map_view_of_section(
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

    return base_address;
}

auto
win::x64_nt_unmap_view_of_section(
    const void* const process_handle,
    const u64         base_address
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtUnmapViewOfSection")

    return static_cast<long_t>(
        x64_call(
            function_address,
            2,
            x64_from_handle(process_handle),
            base_address
        )
    );
}

auto
win::x64_nt_extend_section(
    const void* const section_handle,
    const u64         size
) noexcept -> long_t
{
    ZEN_DECL_HEAVENSGATE_METADATA("ntdll.dll", "NtExtendSection")

    return static_cast<long_t>(
        x64_call(
            function_address,
            2,
            x64_from_handle(section_handle),
            size
        )
    );
}
#endif //ZEN_OS_32_BIT
