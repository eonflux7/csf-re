#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(uint32_t*, g_SceneEntryBuffer, 0x00899744);
CSF_GLOBAL(uint32_t, g_SceneEntryCapacity, 0x00899748);
CSF_GLOBAL(uint32_t, g_SceneEntryCount, 0x0089974C);

static uint32_t __cdecl SceneEntries_ResetBuffer()
{
    uint32_t result = reinterpret_cast<uint32_t>(g_SceneEntryBuffer);

    g_SceneEntryCapacity = 0;
    g_SceneEntryCount = 0;

    if (result)
    {
        result = static_cast<uint32_t>(CoreMemory_Free(reinterpret_cast<void*>(result)));
        g_SceneEntryBuffer = nullptr;
    }

    return result;
}

HOOK(0x004755F0, SceneEntries_ResetBuffer, "auto");
