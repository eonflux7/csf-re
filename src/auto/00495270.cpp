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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static void __fastcall C_00833b88_RequestPrimaryAction(C_00833b88* self, void* /*edx*/)
{
    if ((CSF_AT(uint8_t, self, 0x2E4) & 0x10) == 0)
        return;

    int actionFlags = CSF_AT(int, self, 0x2E0);
    CSF_AT(uint8_t, self, 0x3DD) = 1;

    if ((actionFlags & 0x2000) == 0)
        return;

    // Handle table lookup: the handle argument is the address of the embedded
    // sub-object at +0x1B4 (add ecx, 0x1B4; push ecx).
    int handleAddress = static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(static_cast<void*>(self)) + 0x1B4));
    int resolvedEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
    if (resolvedEntry == 0)
        return;

    int controller = CSF_AT(int, reinterpret_cast<void*>(resolvedEntry), 0x44);
    if (controller == 0)
        return;

    // Virtual slot 200 / 4 = 50, called with argument 0.
    void* controllerObj = reinterpret_cast<void*>(controller);
    int actionResult = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, controllerObj, 0)[50])(controllerObj, nullptr, 0);
    if (actionResult != 0)
        CSF_AT(uint8_t, reinterpret_cast<void*>(actionResult), 0x16) = 0;
}

HOOK(0x495270, C_00833b88_RequestPrimaryAction, "auto");
