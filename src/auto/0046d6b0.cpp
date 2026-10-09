#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Walks the records [first, last) with a 28-byte stride, assigning the source string
// (position 0, full length) into each record's string. Returns the last call's result.
static int __cdecl MissionScript_AssignStringRangeFromSource(C_0082d410* first, C_0082d410* last, int source)
{
    int result = 0;
    for (C_0082d410* record = first; record != last;
         record = reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(record) + 28))
    {
        result = reinterpret_cast<int>(AssignStringSubstring(record, nullptr,
                                                             reinterpret_cast<const void*>(source),
                                                             0, 0xFFFFFFFFu));
    }
    return result;
}

HOOK(0x0046D6B0, MissionScript_AssignStringRangeFromSource, "auto");
