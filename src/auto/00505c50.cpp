#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Imported WaitForSingleObject, read through its IAT slot at 0x82D20C.
using WaitForSingleObjectFn = uint32_t(__stdcall*)(void*, uint32_t);
CSF_GLOBAL(WaitForSingleObjectFn, g_pWaitForSingleObject, 0x0082D20C);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, sub_652010, 0x00652010, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6BE9C0, 0x006BE9C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_006cabc0_GetField0098, 0x006CABC0, (void* self, void*));

// Drains the pending-unload flag: waits on the scene handle, resolves the
// current scene object and releases its Field0098 child until the flag clears.
static char __fastcall CSceneLoader_DrainPendingUnloads(CSceneLoader* self, void* /*edx*/)
{
    // Virtual slot 0x24 (index 9), no typed wrapper: called first, result unused.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    char result = static_cast<char>(CSF_AT(uint8_t, self, 0xB5));
    while (result)
    {
        g_pWaitForSingleObject(CSF_AT(void*, self, 0x110), 0xA);

        int globalInt = GetGlobalInt();
        int sceneObject = sub_652010(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);
        int field0098 = C_006cabc0_GetField0098(
            reinterpret_cast<void*>(static_cast<uintptr_t>(sceneObject)), nullptr);
        if (field0098)
        {
            sub_6BE9C0(reinterpret_cast<void*>(static_cast<uintptr_t>(field0098)), nullptr);
        }

        result = static_cast<char>(CSF_AT(uint8_t, self, 0xB5));
    }
    return result;
}

HOOK(0x00505C50, CSceneLoader_DrainPendingUnloads, "auto");
