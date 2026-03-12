cmake_minimum_required(VERSION 3.20)
project(zen LANGUAGES CXX)

set(ZEN_HEADERS
  # coff directory
  include/zen/coff/file_header.hpp
  include/zen/coff/reloc.hpp
  include/zen/coff/section_header.hpp
  include/zen/coff/string.hpp
  include/zen/coff/symbol.hpp
  include/zen/coff/version.hpp
  # core directory
  include/zen/core/bit.hpp
  include/zen/core/definitions.h
  include/zen/core/fnv.hpp
  include/zen/core/requirements.hpp
  include/zen/core/xors.hpp
  # nt directory
  include/zen/nt/directories/delay_load.hpp
  include/zen/nt/directories/exports.hpp
  include/zen/nt/directories/iat.hpp
  include/zen/nt/directories/imports.hpp
  include/zen/nt/directories/relocs.hpp
  include/zen/nt/directories/tls.hpp
  include/zen/nt/data_directories.hpp
  include/zen/nt/data_directory.hpp
  include/zen/nt/dos_header.hpp
  include/zen/nt/image.hpp
  include/zen/nt/iterator.hpp
  include/zen/nt/nt_headers.hpp
  include/zen/nt/optional_header.hpp
)

set(ZEN_PLATFORM_HEADERS
  # common directory
  include/zen/platform/common/com_ptr.hpp
  include/zen/platform/common/handle_guard.hpp
  include/zen/platform/common/input.hpp
  include/zen/platform/common/memory_guard.hpp
  include/zen/platform/common/memory_section.hpp
  include/zen/platform/common/protection_guard.hpp
  include/zen/platform/common/status_code.hpp
  # rtl directory
  include/zen/platform/rtl/client_id.hpp
  include/zen/platform/rtl/context.hpp
  include/zen/platform/rtl/io_counters.hpp
  include/zen/platform/rtl/io_status_block.hpp
  include/zen/platform/rtl/large_integer.hpp
  include/zen/platform/rtl/ldr_data_table_entry.hpp
  include/zen/platform/rtl/list_entry.hpp
  include/zen/platform/rtl/memory_basic_information.hpp
  include/zen/platform/rtl/object_attributes.hpp
  include/zen/platform/rtl/object_basic_information.hpp
  include/zen/platform/rtl/peb.hpp
  include/zen/platform/rtl/peb_ldr_data.hpp
  include/zen/platform/rtl/process_basic_information.hpp
  include/zen/platform/rtl/section_basic_information.hpp
  include/zen/platform/rtl/string.hpp
  include/zen/platform/rtl/system_process_information.hpp
  include/zen/platform/rtl/system_thread_information.hpp
  include/zen/platform/rtl/thread_basic_information.hpp
  include/zen/platform/rtl/user_process_parameters.hpp
  include/zen/platform/rtl/vm_counters.hpp
  # platform directory
  include/zen/platform/constants.hpp
  include/zen/platform/heavensgate.hpp
  include/zen/platform/syscall.hpp
  include/zen/platform/windows.hpp
)

if (NOT WIN32)
  add_library(${PROJECT_NAME} INTERFACE ${ZEN_HEADERS})

  target_include_directories(${PROJECT_NAME}
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )

  target_compile_definitions(${PROJECT_NAME}
    INTERFACE
      ZEN_IMAGE_RELOC_INFO_COLLECTION
      ZEN_IMAGE_IMPORT_INFO_COLLECTION
      ZEN_IMAGE_EXPORT_INFO_COLLECTION
  )
else()
  # Enable MASM for x64 builds
  # Force ml64.exe even when using clang-cl
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_ASM_MASM_COMPILER "ml64.exe")
    enable_language(ASM_MASM)
  endif()

  # Source files (common to all platforms)
  set(ZEN_SOURCES
    src/platform/memory_section.cpp
    src/platform/syscall.cpp
    src/platform/windows.cpp
  )

  # Platform-specific sources
  if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # Win32 (x86) - include heavensgate
    list(APPEND ZEN_SOURCES src/platform/heavensgate.cpp)
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # x64 - include MASM assembly
    list(APPEND ZEN_SOURCES src/platform/syscall64.asm)
  endif()

  add_library(${PROJECT_NAME} STATIC ${ZEN_SOURCES} ${ZEN_HEADERS} ${ZEN_PLATFORM_HEADERS})

  target_include_directories(${PROJECT_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )

  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      ZEN_IMAGE_RELOC_INFO_COLLECTION
      ZEN_IMAGE_IMPORT_INFO_COLLECTION
      ZEN_IMAGE_EXPORT_INFO_COLLECTION
      UNICODE
      _UNICODE
    PRIVATE
      $<$<CONFIG:Debug>:_DEBUG>
      $<$<CONFIG:Release>:NDEBUG>
      $<$<CONFIG:RelWithDebInfo>:NDEBUG>
      $<$<CONFIG:MinSizeRel>:NDEBUG>
  )

  if (CMAKE_SIZEOF_VOID_P EQUAL 4)
      target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32)
  endif()

  if (MSVC)
      # Common flags for both MSVC and clang-cl
      target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:/W3>                                 # Warning level 3
        $<$<COMPILE_LANGUAGE:CXX>:/sdl>                                # SDL checks
        $<$<COMPILE_LANGUAGE:CXX>:/permissive->                        # Conformance mode
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:/Gy>        # Function-level linking
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:/Oi>        # Intrinsic functions
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RelWithDebInfo>>:/Gy>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RelWithDebInfo>>:/Oi>
      )

      if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${PROJECT_NAME} PRIVATE
          $<$<COMPILE_LANGUAGE:CXX>:/MP>                                 # Multi-processor compilation
          $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:/GL>        # Whole program optimization
        )
      endif()

      # Clang-cl: use LTO instead of /GL
      if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE
          $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:-flto=thin>
        )
      endif()

      # Runtime library options
      # Note: CMake handles this via CMAKE_MSVC_RUNTIME_LIBRARY
      # Set to "MultiThreaded$<$<CONFIG:Debug>:Debug>" for static CRT (/MT, /MTd)
      # Set to "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" for dynamic CRT (/MD, /MDd)
  endif()

  set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
)
