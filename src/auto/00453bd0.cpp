#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, sub_453260, 0x00453260, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Initializes a text entry from another one: copies the sub-object at +4 through sub_453260,
// then resets the string length/capacity fields and assigns the string at +20 from the source.
static int __stdcall TextEntry_CopyFrom(int a1, int a2)
{
    int result = 0;

    if (a1 != 0)
    {
        std::uint8_t* obj = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a1));
        std::uint8_t* src = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a2));

        *reinterpret_cast<void***>(obj) = CSF_VTBL(0x00831278);
        sub_453260(obj + 4, nullptr, reinterpret_cast<std::uint32_t*>(src + 4));

        CSF_AT(std::uint32_t, obj, 0x2C) = 15;
        CSF_AT(std::uint32_t, obj, 0x28) = 0;
        CSF_AT(std::uint8_t, obj, 0x18) = 0;

        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
            AssignStringSubstring(obj + 0x14, nullptr, src + 0x14, 0u, 0xFFFFFFFFu)));
    }

    return result;
}

HOOK(0x453BD0, TextEntry_CopyFrom, "auto");
