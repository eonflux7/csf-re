#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// std::_String_base::assign-style helper: __thiscall, 3 stack args (src, pos, count), ret 12.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, uint32_t position, uint32_t count));

// Copies a string-based save entry record from a2 into a1.
// a1 layout: string at +0x1C (base +0x00..+0x1B), +0x14 length, +0x18 capacity, +0x04 flag byte.
// Copies dword at +0x1C and byte at +0x20 from a2 into a1. Returns the copied dword.
static int __stdcall MenuSaveGame_CopySaveEntry(int a1, int a2)
{
    int result = 0;
    if (a1 != 0)
    {
        uint8_t* dst = reinterpret_cast<uint8_t*>(a1);
        uint8_t* src = reinterpret_cast<uint8_t*>(a2);

        CSF_AT(uint32_t, dst, 0x18) = 15;
        CSF_AT(uint32_t, dst, 0x14) = 0;
        CSF_AT(uint8_t, dst, 0x04) = 0;
        AssignStringSubstring(dst, nullptr, src + 0x1C, 0, 0xFFFFFFFFu);

        result = static_cast<int>(CSF_AT(uint32_t, src, 0x1C));
        CSF_AT(uint32_t, dst, 0x1C) = static_cast<uint32_t>(result);
        CSF_AT(uint8_t, dst, 0x20) = CSF_AT(uint8_t, src, 0x20);
    }
    return result;
}

HOOK(0x00544C60, MenuSaveGame_CopySaveEntry, "auto");
