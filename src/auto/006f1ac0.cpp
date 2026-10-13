#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ReleaseParentState, 0x006C9F40, (void* self, void* edx));

static void __fastcall C_00862490_ReleaseOwnedHandles(C_00862490* self, void* /*edx*/)
{
    // Child object held at +0x40: release it through its vtable slot 0 with arg 1.
    void* ownedChild = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0040));
    self->vtbl = CSF_VTBL(0x00862490);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_VTBL(0x00862488);
    self->field_0008 = (uint32_t)(uintptr_t)CSF_VTBL(0x0086246C);
    if (ownedChild) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedChild, 0)[0])(ownedChild, nullptr, 1);
        self->field_0040 = 0;
    }

    // Registry handle at +0x20: remove it from the scene object registry via vtable slot 0x2C / 4.
    if (self->field_0020) {
        int globalInt = GetGlobalInt();
        int registryValue = SceneLoader_GetObjectRegistry(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);
        void* registry = reinterpret_cast<void*>(static_cast<uintptr_t>(registryValue));
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, registry, 0)[0x2C / 4])(
            registry, nullptr, self->field_0020);
        self->field_0020 = 0;
    }

    RpPrtStd_ResetBaseState(self, nullptr);
    RpPrtStd_ReleaseParentState(self, nullptr);
}

HOOK(0x006F1AC0, C_00862490_ReleaseOwnedHandles, "auto");
