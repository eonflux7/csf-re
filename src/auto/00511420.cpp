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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static uint32_t* __fastcall CMenuManager_AllocateAuxBlock(C_008407a8* self, void* /*edx*/)
{
    // Allocate a 120-byte block from the game heap and link it to the menu manager.
    std::uint32_t* result = static_cast<std::uint32_t*>(Mem_Alloc(120));
    if (result)
    {
        // The new block's first dword is its vtable pointer.
        *result = static_cast<std::uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0084077C)));
        self->field_007c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(result));
    }
    else
    {
        self->field_007c = 0;
        return nullptr;
    }
    return reinterpret_cast<uint32_t*>(result);
}

HOOK(0x00511420, CMenuManager_AllocateAuxBlock, "auto");
