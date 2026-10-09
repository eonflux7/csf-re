#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value8B0554, 0x008B0554);

// Releases the cached scene-mission object held in g_Value8B0554: if set, calls its
// vtable slot 0 with flag 1 (destroy-with-free), then clears the global.
static int __cdecl SceneMissionSetup_ReleaseCachedInstance()
{
    int result = 0;
    if (g_Value8B0554 != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value8B0554)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_Value8B0554 = 0;
    }
    return result;
}

HOOK(0x004E8C60, SceneMissionSetup_ReleaseCachedInstance, "auto");
