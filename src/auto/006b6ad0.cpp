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

// Game heap allocator (cdecl, caller pops). Returns the block address as an int.
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Initializer for the visibility-resources object (__fastcall: ecx=this, edx unused).
CSF_FUNC(int, __fastcall, sub_6B7F90, 0x006B7F90, (void* self, void* edx));

// Allocates a 44-byte visibility-resources object from the game heap, initializes it,
// and returns it. Returns nullptr when the allocation fails.
static C_00860388* __stdcall GfxScene_GetVisibilityResources()
{
    const int rawBlock = CoreMemory_AllocUntagged(44);
    if (!rawBlock)
        return nullptr;

    C_00860388* visibilityResources =
        reinterpret_cast<C_00860388*>(static_cast<std::uintptr_t>(rawBlock));

    sub_6B7F90(visibilityResources, nullptr);
    visibilityResources->vtbl = CSF_VTBL(0x00861F94);
    return visibilityResources;
}

HOOK(0x006B6AD0, GfxScene_GetVisibilityResources, "auto");
