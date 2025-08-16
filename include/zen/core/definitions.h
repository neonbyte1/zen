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
#if !defined(ZEN_CORE_DEFINITIONS_H)
#define ZEN_CORE_DEFINITIONS_H

#undef ZEN_TARGET_32_BIT // defined when the target architecture is 32-bit
#undef ZEN_TARGET_64_BIT // defined when the target architecture is 64-bit
#undef ZEN_TARGET_ARM    // defined when compiling with an ARM compiler
#undef ZEN_OS_APPLE      // defined when compiling with macOS
#undef ZEN_OS_LINUX      // defined when compiling with a linux system
#undef ZEN_OS_POSIX      // defined when compiling under unix/posix systems
#undef ZEN_OS_WINDOWS    // defined when compiling with Windows
#undef ZEN_CXX           // defined when compliing with a C++ compiler
#undef ZEN_CXX_CLANG     // defined when compiling with the clang compiler
#undef ZEN_CXX_GCC       // defined when compiling with the GCC compiler
#undef ZEN_CXX_INTEL     // defined when compiling with the Intel Compiler (ICC)
#undef ZEN_CXX_MINGW     // defined when compiling with the MinGW compiler
#undef ZEN_CXX_MUSL      // defined when compiling with the musl compiler
#undef ZEN_CXX_MSVC      // defined when compiling with the Windows compiler (MSVC)

#if defined(_MSC_VER) || defined(__MINGW64__) || defined(__MINGW32__)
#   define ZEN_OS_WINDOWS 1
#elif defined(__linux__)
#   define ZEN_OS_LINUX 1
#elif defined(__unix__) || defined(__posix__)
#   if defined(__APPLE__) && defined(__MACH__)
#       define ZEN_OS_APPLE 1
#   else
#       define ZEN_OS_POSIX 1
#   endif
#else
#   error "Unable to detect the target system"
#endif

#if defined(__aarch64__) || defined(__arm__) || defined(__armv7__)
#   define ZEN_TARGET_ARM 1
#endif

#if defined(_WIN64) || defined(_M_X64) || defined(__MINGW64__) || defined(__aarch64__) || defined(__x86_64__) || defined(__amd64__)
#   define ZEN_TARGET_64_BIT 1
#elif defined(_WIN32) || defined(_M_IX86) || defined(__MINGW32__) || defined(__arm__) || defined(__armv7__) || defined(__i386__)
#   define ZEN_TARGET_32_BIT 1
#else
#   error "Unable to detect the target system architecture"
#endif

#if defined(_MSC_VER)
#   if defined(__clang__)
#       define ZEN_CXX_CLANG 1
#   elif defined(__MINGW64__) || defined(__MINGW32__)
#       define ZEN_CXX_MINGW 1
#   elif defined(__INTEL_COMPILER)
#       define ZEN_CXX_INTEL 1
#   else
#       define ZEN_CXX_MSVC 1
#   endif
#elif defined(__GNUC__)
#   define ZEN_CXX_GCC 1
#elif defined(__clang__)
#   define ZEN_CXX_CLANG 1
#elif defined(__musl__)
#   define ZEN_CXX_MUSL 1
#else
#   if defined(ZEN_CXX_MSVC) || defined(ZEN_CXX_INTEL)
#       pragma warning "Unable to detect a known compiler"
#   else
#       warning "Unable to detect a known compiler"
#   endif
#endif

#if !defined(ZEN_WIN32_CC)
#   if defined(ZEN_OS_WINDOWS)
#       define ZEN_WIN32_CC __stdcall
#   else
#       define ZEN_WIN32_CC
#   endif
#endif //ZEN_WIN32_CC

#if !defined(ZEN_FORCEINLINE)
#   if defined(ZEN_OS_WINDOWS) && !defined(ZEN_CXX_MINGW)
#       define ZEN_FORCEINLINE __forceinline
#   else
#       define ZEN_FORCEINLINE __attribute__((always_inline)) inline
#   endif
#endif //ZEN_FORCEINLINE

#if !defined(ZEN_WIN32_DLL_PROCESS_DETACH)
#   define ZEN_WIN32_DLL_PROCESS_DETACH 0
#endif //ZEN_WIN32_DLL_PROCESS_DETACH

#if !defined(ZEN_WIN32_DLL_PROCESS_ATTACH)
#   define ZEN_WIN32_DLL_PROCESS_ATTACH 1
#endif //ZEN_WIN32_DLL_PROCESS_ATTACH

#if defined(ZEN_OS_WINDOWS)
#   if !defined(NOMINMAX)
#       define NOMINMAX
#   endif //NOMINMAX
#   if !defined(WIN32_LEAN_AND_MEAN)
#       define WIN32_LEAN_AND_MEAN
#   endif //WIN32_LEAN_AND_MEAN
#endif //_MSC_VER

#if !defined(ZEN_WIN32_VAR_LEN)
#   if defined(ZEN_CXX_MSVC) || defined(ZEN_CXX_INTEL)
#       define ZEN_WIN32_VAR_LEN 1
#   else
//#       pragma warning(disable : 4200)
#       define ZEN_WIN32_VAR_LEN 1
#   endif
#endif //ZEN_WIN32_VAR_LEN

#if !defined(ZEN_WIN32_LEN_SHORT_STR)
#   define ZEN_WIN32_LEN_SHORT_STR 8
#endif //ZEN_WIN32_LEN_SHORT_STR

#if !defined(ZEN_COFF_ALIGNMENT)
#   define ZEN_COFF_ALIGNMENT(ns) \
        _Pragma("pack(push, 1)")  \
        namespace ns {
#endif //ZEN_COFF_ALIGNMENT

#if !defined(ZEN_WIN32_ALIGNMENT)
#   define ZEN_WIN32_ALIGNMENT(ns) \
        _Pragma("pack(push, 4)") \
        namespace ns {
#endif //ZEN_WIN32_ALIGNMENT

#if !defined(ZEN_RESTORE_ALIGNMENT)
#   define ZEN_RESTORE_ALIGNMENT() } \
        _Pragma("pack(pop)")
#endif //ZEN_RESTORE_ALIGNMENT

#if !defined(ZEN_DECLSPEC_ALIGN)
#   if defined(ZEN_CXX_MSVC) || defined(ZEN_CXX_INTEL)
#       define ZEN_DECLSPEC_ALIGN(x) __declspec(align(x))
#   else
#       define ZEN_DECLSPEC_ALIGN(x) __attribute__((aligned(x)))
#   endif
#endif //ZEN_DECLSPEC_ALIGN

#if !defined(ZEN_TYPED_THISCALL_HOOK_ARGS_X86)
#   define ZEN_TYPED_THISCALL_HOOK_ARGS_X86(Type) \
        Type* ecx, void* edx
#endif //ZEN_TYPED_THISCALL_HOOK_ARGS_X86

#if !defined(ZEN_TYPED_THISCALL_HOOK_ARGS_X64)
#   define ZEN_TYPED_THISCALL_HOOK_ARGS_X64(Type) \
        Type* rcx, void* rdx
#endif //ZEN_TYPED_THISCALL_HOOK_ARGS_X64

#if !defined(ZEN_THISCALL_HOOK_ARGS_X86)
#   define ZEN_THISCALL_HOOK_ARGS_X86 ZEN_TYPED_THISCALL_HOOK_ARGS_X86(void)
#endif //ZEN_THISCALL_HOOK_ARGS_X86

#if !defined(ZEN_THISCALL_HOOK_ARGS_X64)
#   define ZEN_THISCALL_HOOK_ARGS_X64 ZEN_TYPED_THISCALL_HOOK_ARGS_X64(void)
#endif //ZEN_THISCALL_HOOK_ARGS_X64

#if !defined(ZEN_TYPED_THISCALL_HOOK_ARGS)
#   if defined(ZEN_TARGET_32_BIT)
#       define ZEN_TYPED_THISCALL_HOOK_ARGS(Type) ZEN_TYPED_THISCALL_HOOK_ARGS_X86(Type)
#   else
#       define ZEN_TYPED_THISCALL_HOOK_ARGS(Type) ZEN_TYPED_THISCALL_HOOK_ARGS_X64(Type)
#   endif
#endif //ZEN_TYPED_THISCALL_HOOK_ARGS

#if !defined(ZEN_THISCALL_HOOK_ARGS)
#   if defined(ZEN_TARGET_32_BIT)
#       define ZEN_THISCALL_HOOK_ARGS ZEN_THISCALL_HOOK_ARGS_X86
#   else
#       define ZEN_THISCALL_HOOK_ARGS ZEN_THISCALL_HOOK_ARGS_X64
#   endif
#endif //ZEN_TYPED_THISCALL_HOOK_ARGS

#if !defined(ZEN_EXTERN)
#   if defined(ZEN_CXX)
#       define ZEN_EXTERN extern "C"
#   else
#       define ZEN_EXTERN extern
#   endif
#endif //ZEN_EXTERN

#if !defined(UNUSED_PARAM)
#   define UNUSED_PARAM(p) (void)(p)
#endif //UNUSED_PARAM

#if defined(ZEN_INCLUDE_WINDOWS_HEADER) && defined(ZEN_OS_WINDOWS)
#   if defined(ZEN_CXX_MINGW)
#       include <windows.h>
#   else
#       include <Windows.h>
#   endif
#   if defined(ZEN_CXX_MSVC)
#       pragma message("--------------------------------------------------------------------------------")
#       pragma message("You're including Windows.h which might adds additional code to your binary")
#       pragma message("--------------------------------------------------------------------------------")
#   else
#       warning "--------------------------------------------------------------------------------"
#       warning "You're including Windows.h which might adds additional code to your binary"
#       warning "--------------------------------------------------------------------------------"
#   endif
#endif

#if defined(__cplusplus)
#   define ZEN_CXX
#   include <cstddef>
#   include <cstdint>
#else
#   include <stdbool.h>
#   include <stdint.h>
#   include <stddef.h>
#endif

#if !defined(ZEN_STD_TYPE)
#   if defined(ZEN_CXX)
#       define ZEN_STD_TYPE(Type) std::Type
#   else
#       define ZEN_STD_TYPE(Type) Type
#   endif
#endif //ZEN_STD_TYPE

#if !defined(ZEN_STD_TYPEDEF)
#   if defined(ZEN_CXX)
#       define ZEN_STD_TYPEDEF(Alias, Type) using Alias = Type
#   else
#       define ZEN_STD_TYPEDEF(Alias, Type) typedef Type Alias;
#   endif
#endif //ZEN_STD_TYPEDEF

ZEN_STD_TYPEDEF(i8,   ZEN_STD_TYPE(int8_t));
ZEN_STD_TYPEDEF(u8,   ZEN_STD_TYPE(uint8_t));
ZEN_STD_TYPEDEF(i16,  ZEN_STD_TYPE(int16_t));
ZEN_STD_TYPEDEF(u16,  ZEN_STD_TYPE(uint16_t));
ZEN_STD_TYPEDEF(i32,  ZEN_STD_TYPE(int32_t));
ZEN_STD_TYPEDEF(u32,  ZEN_STD_TYPE(uint32_t));
ZEN_STD_TYPEDEF(i64,  ZEN_STD_TYPE(int64_t));
ZEN_STD_TYPEDEF(u64,  ZEN_STD_TYPE(uint64_t));
ZEN_STD_TYPEDEF(iptr, ZEN_STD_TYPE(intptr_t));
ZEN_STD_TYPEDEF(uptr, ZEN_STD_TYPE(uintptr_t));
ZEN_STD_TYPEDEF(szt,  ZEN_STD_TYPE(size_t));
ZEN_STD_TYPEDEF(f32,  float);
ZEN_STD_TYPEDEF(f64,  double);

typedef union b1_t
{
    bool b;
    char c;
    ::i8 i8;
    ::u8 u8;
} b1;

typedef union b2_t
{
    ::i16   i16;
    ::u16   u16;
#if defined(ZEN_OS_WINDOWS)
    wchar_t wc;
#endif
    ::u8    bytes[2];

    struct
    {
        b1 lo;
        b1 hi;
    } bits;
} b2;

typedef union b4_t
{
    ::i32 i32;
    ::u32 u32;
    ::f32 f32;
    ::u8  bytes[4];
    ::u16 ds[2];

    struct
    {
        b2 lo;
        b2 hi;
    } bits;
} b4;

typedef union b8_t
{
    ::i64 i64;
    ::u64 u64;
    ::f64 f64;
    ::u8  bytes[8];
    ::u32 dw[2];

    struct
    {
        b4 lo;
        b4 hi;
    } bits;
} b8;

#if defined(ZEN_CXX)
static_assert(sizeof(b1) == sizeof(u8));
static_assert(sizeof(b2) == sizeof(u16));
static_assert(sizeof(b4) == sizeof(u32));
static_assert(sizeof(b8) == sizeof(u64));
#endif

#if !defined(ZEN_WIN32_CONTAINING_RECORD)
#   define ZEN_WIN32_CONTAINING_RECORD(Address, Type, Field) \
        ((Type*)((char*)(Address) - (::uptr)(&((Type*)0)->Field)))
#endif //WIN32_CONTAINING_RECORD

#if !defined(MACRO_CONCAT)
#   define MACRO_CONCAT_IMPL(A, B) A##B
#   define MACRO_CONCAT(A, B) \
        MACRO_CONCAT_IMPL(A, B)
#endif //MACRO_CONCAT

#if !defined(PAD)
#   if defined(ZEN_CXX)
#       define PAD(N) \
            private: \
                ::u8 MACRO_CONCAT(_pad, __COUNTER__)[N]{}; \
            public:
#   else
#       define PAD(N) ::u8 MACRO_CONCAT(_pad, __COUNTER__)[N]
#   endif
#endif //PAD

#endif //ZEN_CORE_DEFINITIONS_H
