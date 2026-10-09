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

// Assigns the sub-object at entry+4 from the source entry's sub-object (thiscall, one stack arg).
CSF_FUNC(int, __fastcall, AssignEntrySubObject, 0x004525E0, (void* self, void* edx, int a2));

// Substring assignment on the string at entry+20 (thiscall, source/position/count).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies entries in [firstEntry, endEntry) from sourceEntry, stepping 48 bytes per entry.
// Returns endEntry, as the original does.
static int __cdecl CopyEntryRangeFromSource(int firstEntry, int endEntry, int sourceEntry)
{
    int result = endEntry;
    for (int entry = firstEntry; entry != endEntry; entry += 48)
    {
        AssignEntrySubObject(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entry + 4)),
                             nullptr,
                             sourceEntry + 4);
        AssignStringSubstring(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entry + 20)),
                              nullptr,
                              reinterpret_cast<const void*>(static_cast<std::uintptr_t>(sourceEntry + 20)),
                              0,
                              0xFFFFFFFFu);
        result = endEntry;
    }
    return result;
}

HOOK(0x004548B0, CopyEntryRangeFromSource, "auto");
