#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int16_t, __fastcall, InvokeControlAction, 0x004921D0, (void* self, void* edx, int a2));

static int __stdcall ResolveFlaggedControlAction(int action, int handle)
{
    int result = handle;
    if (CSF_AT(int, reinterpret_cast<void*>(handle), 0x8) >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        if (result)
        {
            if ((CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x170) & 0x2000) != 0)
                return InvokeControlAction(reinterpret_cast<void*>(result), nullptr, action);
        }
    }
    return result;
}

HOOK(0x004481B0, ResolveFlaggedControlAction, "auto");
