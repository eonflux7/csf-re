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

// Original: GetGlobalState (0x50D190), __cdecl, no args, returns a global state pointer.
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());

// Original: CGuiControl_Construct (0x71A170), __thiscall with one stack arg, ret 4.
CSF_FUNC(int, __fastcall, CGuiControl_Construct, 0x71A170, (void* self, void* edx, void* a2));

// Vtable of CGuiCharacterPreview.
CSF_GLOBAL(void*, g_vtbl_CGuiCharacterPreview, 0x008432B0);

// CGuiCharacterPreview constructor (__thiscall, ret 0).
static CGuiCharacterPreview* __fastcall CGuiCharacterPreview_Construct(CGuiCharacterPreview* self, void* /*edx*/)
{
    // v2 = GetGlobalState(); obj = v2->field_14; v3 = obj->vtbl[21](obj)
    void* globalState = reinterpret_cast<void*>(GetGlobalState());
    void* managerObj = CSF_AT(void*, globalState, 0x14);
    void* sceneRoot = reinterpret_cast<void*>(
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, managerObj, 0)[0x54 / 4]))(managerObj, nullptr));

    // v4 = v3->vtbl[4](v3)
    void* sceneRootChild = reinterpret_cast<void*>(
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, sceneRoot, 0)[0x10 / 4]))(sceneRoot, nullptr));

    CGuiControl_Construct(self, nullptr, sceneRootChild);

    self->m_pScene = 0;
    self->m_pCamera = 0;
    self->m_pCharacter = 0;
    self->m_pCharClump = 0;
    self->m_pWeaponClump = 0;
    self->m_pLight = 0;
    self->m_angle = 0;

    *reinterpret_cast<void**>(self) = static_cast<void*>(&g_vtbl_CGuiCharacterPreview);
    return self;
}

HOOK(0x00541E70, CGuiCharacterPreview_Construct, "auto");
