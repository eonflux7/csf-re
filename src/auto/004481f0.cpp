#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
static CSF_FUNC(int16_t, __fastcall, GetObjectShortValue, 0x00492CE0, (void* self, void* edx));

static int16_t __stdcall GetFlaggedSceneObjectShortValue(int handle)
{
    if (CSF_AT(int32_t, reinterpret_cast<void*>(handle), 0x8) < 0)
        return 0;

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (resolved == 0)
        return 0;

    void* object = reinterpret_cast<void*>(resolved);
    if ((CSF_AT(uint32_t, object, 0x170) & 0x2000) == 0)
        return 0;

    return GetObjectShortValue(object, nullptr);
}

HOOK(0x004481F0, GetFlaggedSceneObjectShortValue, "auto");
