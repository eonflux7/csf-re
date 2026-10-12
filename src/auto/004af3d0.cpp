#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4AA5E0, 0x004AA5E0, (void* self, void* edx));

static bool __fastcall CoverTask_IsLinkedObjectReady(S_004aa5e0* self, void* /*edx*/)
{
    // Inert reference to the S_004aa5e0 vtable (no store is performed here).
    (void)CSF_VTBL(0x004AA5E0);

    if (self->field_0608 == 0xFFFFFFFFu)
        return false;

    // Sub-object at +0x5DC, passed as this to the state check.
    void* stateObject = reinterpret_cast<uint8_t*>(self) + 0x5DC;
    // Handle is the address of the embedded member at +0x610.
    int handleAddress = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x610);

    if (CoverAttack_CheckState_4ADAE0(stateObject, nullptr, handleAddress) == 0)
        return false;

    if (self->field_0684 == 0)
        return false;

    // Negative values (sign bit set) are rejected.
    if ((self->field_0618 & 0x80000000u) != 0)
        return false;

    auto* sceneObject = reinterpret_cast<C_00833b88*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress));

    if (sceneObject == nullptr)
        return false;

    if (SceneObj_IsInactive(sceneObject, nullptr) != 0)
        return false;

    // The original passes the same resolved object on; no separate class type is needed.
    void* sceneObjectRaw = sceneObject;
    if (SceneObject_PassesFirstCheck(sceneObjectRaw, nullptr) != 0)
        return false;

    return sub_4AA5E0(self, nullptr);
}

HOOK(0x004AF3D0, CoverTask_IsLinkedObjectReady, "auto");
