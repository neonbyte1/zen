#pragma once

#include <zen/platform/rtl/large_integer.hpp>

namespace zen::rtl {
enum struct section_info : u32
{
    basic,      // SectionBasicInformation
    image,      // SectionImageInformation
    relocation, // SectionRelocationInformation
    last_index, // MaxSectionInfoClass
};

enum struct section_inherit : u32
{
    view_share = 1,
    view_unmap,
};

template<bool X64 = detail::is_64_bit>
struct section_basic_information
{
    va_t<X64>     base_address{};
    u32           allocation_attributes{};
    large_integer maximum_size{};
};
} //namespace zen::rtl
