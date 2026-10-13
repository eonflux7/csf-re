#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __cdecl HudSpritesLayout_IsSceneObjectEligible(C_00833b88* object)
{
    if (!object || !object->vGetField016c())
        return false;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* entries = UmsVirtualProcessor_GetDefaultDestination(manager, nullptr);
    const uint32_t entryCount =
        static_cast<uint32_t>(GetEntryCount(entries, nullptr));

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        manager = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        entries = UmsVirtualProcessor_GetDefaultDestination(manager, nullptr);
        auto* entry = reinterpret_cast<uint8_t*>(
            GetEntryAt(entries, nullptr, static_cast<int>(index)));

        if (CSF_AT(int, entry, 0x00) == 2
            && !CSF_AT(uint8_t, entry, 0x28)
            && CSF_AT(int, entry, 0x1C) == g_CurrentSceneIndex
            && reinterpret_cast<C_00833b88*>(
                   CHandleTable_Resolve(
                       g_pCurrentScene, nullptr,
                       reinterpret_cast<int>(entry + 0x10))) == object)
        {
            return false;
        }
    }

    const uint32_t objectType = object->field_0050;
    if (objectType == 3)
        return static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)) == 0;

    return objectType == 24;
}

HOOK(0x00436BA0, HudSpritesLayout_IsSceneObjectEligible, "auto");
