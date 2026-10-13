#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Initializer for the freshly allocated 92-byte text-database record (__fastcall: ecx=a1, edx=a2).
CSF_FUNC(int, __fastcall, sub_699960, 0x00699960, (int a1, int a2));

// Allocates a 92-byte block from the game heap and hands it to sub_699960; returns 0 if allocation fails.
static int __cdecl TextDatabase_AllocAndInitRecord()
{
    void* recordObject = Mem_Alloc(92);
    if (recordObject)
        return sub_699960(static_cast<int>(reinterpret_cast<std::uintptr_t>(recordObject)), 0);
    return 0;
}

HOOK(0x004EEFB0, TextDatabase_AllocAndInitRecord, "auto");
