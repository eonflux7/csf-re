#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6B16E0, 0x006B16E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value_8BB450, 0x008BB450);

// Releases the cached instance in g_Value_8BB450 if one exists: runs its
// teardown routine, frees the block through the game heap, and clears the
// global pointer. Returns the heap free result, or 0 when nothing was cached.
static int __cdecl ReleaseCachedValueInstance()
{
    if (g_Value_8BB450 == 0)
        return 0;

    void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB450));
    sub_6B16E0(cachedInstance, nullptr);
    int freeResult = Mem_Free(cachedInstance);
    g_Value_8BB450 = 0;
    return freeResult;
}

HOOK(0x6B1F80, ReleaseCachedValueInstance, "auto");
