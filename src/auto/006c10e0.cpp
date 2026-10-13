#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, GfxObject_CheckObjectState, 0x006E2F30, (int objectHandle));
CSF_FUNC(int, __cdecl, GfxIm3d_GetObjectValue, 0x006E2E90, (int entryHandle));
CSF_GLOBAL(std::uint32_t, g_TexDictValueTotal, 0x008BB49C);

static int __cdecl GfxTexDict_AccumulateObjectValue(int objectHandle)
{
    if (objectHandle != 0)
    {
        GfxObject_CheckObjectState(objectHandle);
        const int objectValue = GfxIm3d_GetObjectValue(objectHandle);
        GfxObject_CheckObjectState(objectHandle);
        g_TexDictValueTotal += static_cast<std::uint32_t>(objectValue);
    }
    return objectHandle;
}

HOOK(0x006C10E0, GfxTexDict_AccumulateObjectValue, "auto");
