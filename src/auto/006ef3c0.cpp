#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Initializer for the freshly allocated 56-byte PTank resource (__fastcall: ecx=self, edx unused).
CSF_FUNC(uint32_t*, __fastcall, sub_6EF320, 0x006EF320, (void* self, void* edx));

// Allocates a 56-byte block from the game heap and hands it to sub_6EF320 (tail-called in the original);
// returns nullptr if allocation fails.
static uint32_t* __cdecl PTankCreate_AllocateAndInit()
{
    int allocatedAddress = CoreMemory_AllocUntagged(56);
    if (allocatedAddress)
        return sub_6EF320(reinterpret_cast<void*>(static_cast<std::uintptr_t>(allocatedAddress)), nullptr);
    return nullptr;
}

HOOK(0x006EF3C0, PTankCreate_AllocateAndInit, "auto");
