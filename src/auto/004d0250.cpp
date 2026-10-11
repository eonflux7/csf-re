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

CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));

// Returns true when the control entry at index a2 refers to a live, targetable scene object.
static bool __fastcall C_0083b668_IsEntryTargetable(C_0083b668* self, void* /*edx*/, int a2)
{
    C_008342e8* controlTable = reinterpret_cast<C_008342e8*>(self->field_0014);

    // Entry field at +8 must be non-negative (signed compare).
    if (static_cast<int>(CSF_AT(int, GetControlEntryByIndex(controlTable, nullptr, a2), 0x8)) < 0)
        return false;

    void* scene = g_pCurrentScene;
    int entryHandle = GetControlEntryByIndex(controlTable, nullptr, a2);
    int sceneObjectHandle = CHandleTable_Resolve(scene, nullptr, entryHandle);
    void* sceneObject = reinterpret_cast<void*>(static_cast<uintptr_t>(sceneObjectHandle));

    if (sceneObjectHandle == 0)
        return false;
    if ((CSF_AT(uint32_t, sceneObject, 0x170) & 0x2000u) == 0)
        return false;
    if (CSF_AT(int, sceneObject, 0x50) == 0)
        return false;

    int flagsAddress = C_008342e8_GetControlFlagsAddress(controlTable, nullptr, a2);
    uint8_t* flags = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(flagsAddress));
    if ((*flags & 2) != 0)
        return false;

    if (SceneObject_PassesFirstCheck(sceneObject, nullptr) != 0)
        return false;
    if (SceneObj_IsInactive(sceneObject, nullptr) != 0)
        return false;

    return true;
}

HOOK(0x004D0250, C_0083b668_IsEntryTargetable, "auto");
