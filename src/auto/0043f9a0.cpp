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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Walks an array of 56-byte records from first up to (not including) end and,
// for each record, assigns the whole string at a3 into its string field at +0
// and the string at a3+28 into its string field at +28.
static int __cdecl AssignStringPairRange(int first, int end, int source)
{
    int result = end;
    for (int record = first; record != end; record += 56)
    {
        AssignStringSubstring(reinterpret_cast<C_0082d410*>(static_cast<std::uintptr_t>(record)),
                              nullptr,
                              reinterpret_cast<const void*>(static_cast<std::uintptr_t>(source)),
                              0,
                              static_cast<std::uint32_t>(-1));
        AssignStringSubstring(reinterpret_cast<C_0082d410*>(static_cast<std::uintptr_t>(record) + 28),
                              nullptr,
                              reinterpret_cast<const void*>(static_cast<std::uintptr_t>(source) + 28),
                              0,
                              static_cast<std::uint32_t>(-1));
        result = end;
    }
    return result;
}

HOOK(0x43F9A0, AssignStringPairRange, "auto");
