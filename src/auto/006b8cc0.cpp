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
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(uint32_t, g_TableBase, 0x008BB488);

// Frees the buffer currently held in the slot (table[a1 + 4]) and, if the
// object's 16-bit size field at +0x84 is non-zero, allocates a new buffer of
// that size into the same slot. Returns the new buffer (or 0).
static int __cdecl GfxObjectProps_ResetPropertyBuffer(int a1)
{
    uintptr_t slotAddress = static_cast<uintptr_t>(g_TableBase) + static_cast<uintptr_t>(a1) + 4;
    uint32_t* v1 = *reinterpret_cast<uint32_t**>(slotAddress);
    if (*v1)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(*v1)));
        *v1 = 0;
    }

    auto* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));
    uint16_t bufferSize = CSF_AT(uint16_t, obj, 0x84);
    if (bufferSize == 0)
        return 0;

    uint32_t* v2 = *reinterpret_cast<uint32_t**>(
        static_cast<uintptr_t>(g_TableBase) + static_cast<uintptr_t>(a1) + 4);
    void* block = Mem_Alloc(bufferSize);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(block));
    *v2 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block));
    return result;
}

HOOK(0x6B8CC0, GfxObjectProps_ResetPropertyBuffer, "auto");
