#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_769A60, 0x00769A60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value_8BC1E8, 0x008BC1E8);

// Tears down the global input bindings instance if it exists: runs its
// teardown routine, frees the block through the game heap, and clears the
// global pointer. Returns the heap free result, or 0 when no instance exists.
static int __cdecl ReleaseInputBindingsInstance()
{
    int freeResult = 0;
    if (g_Value_8BC1E8 != 0)
    {
        void* bindingsInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BC1E8));
        sub_769A60(bindingsInstance, nullptr);
        freeResult = Mem_Free(bindingsInstance);
        g_Value_8BC1E8 = 0;
    }
    return freeResult;
}

HOOK(0x769BF0, ReleaseInputBindingsInstance, "auto");
