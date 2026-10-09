#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_434800, 0x00434800, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value_8afc78, 0x008AFC78);

// Destroys the global HUD instance if it exists: runs its teardown routine,
// frees the block through the game heap, and clears the global pointer.
static int __cdecl ReleaseHudInstance()
{
    if (g_Value_8afc78 == 0)
        return 0;

    void* hudInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8afc78));
    sub_434800(hudInstance, nullptr);
    int freeResult = Mem_Free(hudInstance);
    g_Value_8afc78 = 0;
    return freeResult;
}

HOOK(0x436070, ReleaseHudInstance, "auto");
