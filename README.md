# zen

_zen_ is a lightweight and efficient library designed to parse Windows Portable Executable (PE) and Common Object File Format (COFF) files.
It provides a set of functions that allow you to interact with the Windows API on Windows targets, without requiring the inclusion of the `Windows.h` header.

## Features

- Seamlessly parse both PE and COFF formats, including their headers, sections, and other structures
- Supports both little-endian and big-endian systems, ensuring compatibility across diverse platforms
- Provides the core functionality needed to work with the Windows API, without the complexity of the `Windows.h` header
- Enables direct syscall access, offering low-level interaction with the Windows kernel
- Provides support for invoking 64-bit functions within a 32-bit process through the WOW64 Heaven's Gate mechanism
- A minimalistic approach to parsing and interacting with Windows binary formats, designed for simplicity and speed

## Working with the Windows API

> [!TIP]
> The `get_module_handle` and `get_proc_address` functions can be used with ANSI **or** WIDE strings. Both functions are also supporting a 32-bit `FNV-1a` hash as `name` argument.

[url_get_module_handle]: https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea
[url_get_proc_address]: https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress

The [windows.hpp](include/zen/platform/windows.hpp) file contains rebuilds of [GetModuleHandle][url_get_module_handle] and [GetProcAddress][url_get_proc_address]. With these you can obtain and call any Windows API.

## License

[zen](https://github.com/neonbyte1/zen) uses the [BSD-3-Clause](LICENSE.md) license. However, the following components are included with their respective licenses:

- The [coff](include/zen/coff) and [nt](include/zen/nt) directories were initially taken from [linux-pe](https://github.com/can1357/linux-pe) and are licensed under [BSD-3-Clause](https://github.com/can1357/linux-pe/blob/master/LICENSE.md)
- The Heaven's Gate [header](include/zen/platform/heavensgate.hpp) and [source](src/platform/heavensgate.cpp) files were initially taken from [rewolf-wow64ext](https://github.com/rwfpl/rewolf-wow64ext) and are licensed under [LGPL-3.0](https://github.com/rwfpl/rewolf-wow64ext/blob/master/lgpl-3.0.txt)
- The [64-bit syscall assembly code](src/platform/syscall64.asm) was taken from [Blackbone](https://github.com/DarthTon/Blackbone) and is licensed under [MIT](https://github.com/DarthTon/Blackbone/blob/master/LICENSE).
