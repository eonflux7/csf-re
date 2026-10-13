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

// Callee: AssignStringSubstring (__thiscall, 3 stack args, ret 12)
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a file-system descriptor entry from a2 into a1: name string (full copy),
// then the words at +0x1C/+0x20, and the words at +0x24/+0x28.
// Entries are 0x0C bytes apart, so element N lives at base + N * 0x0C.
static std::uint32_t __stdcall CopyFileSystemEntry(C_0082d410* a1, C_0082d410* a2)
{
    std::uint32_t result = 0;

    if (a1) {
        CSF_AT(std::uint32_t, a1, 0x18) = 15;
        CSF_AT(std::uint32_t, a1, 0x14) = 0;
        CSF_AT(std::uint8_t, a1, 0x04) = 0;

        AssignStringSubstring(a1, nullptr, a2, 0, static_cast<std::uint32_t>(-1));

        CSF_AT(std::uint32_t, a1, 0x1C) = CSF_AT(std::uint32_t, a2, 0x1C);
        CSF_AT(std::uint32_t, a1, 0x20) = CSF_AT(std::uint32_t, a2, 0x20);
        CSF_AT(std::uint32_t, a1, 0x24) = CSF_AT(std::uint32_t, a2, 0x24);
        result = CSF_AT(std::uint32_t, a2, 0x28);
        CSF_AT(std::uint32_t, a1, 0x28) = result;
    }
    return result;
}

HOOK(0x69EA90, CopyFileSystemEntry, "auto");
