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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));

// Slot 0 of an owned object's vtable: the release entry, called as (obj, 1).
using OwnedReleaseSlotFn = void(__fastcall*)(void* self, void* edx, int freeFlag);
// Slot 0x2C of the registry object's vtable, called as (registry, handle).
using RegistryUnregisterFn = void(__fastcall*)(void* self, void* edx, uint32_t handle);

static void __fastcall C_00862640_ReleaseHandles(C_00862640* self, void* /*edx*/)
{
    // First owned object: release it through its vtable slot 0 with flag 1.
    void* ownedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0048));
    if (ownedObject) {
        reinterpret_cast<OwnedReleaseSlotFn>(CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0048 = 0;
    }

    // Registered handle: remove it from the scene loader's object registry.
    if (self->field_0020) {
        void* globalObject = reinterpret_cast<void*>(
            static_cast<uintptr_t>(static_cast<uint32_t>(GetGlobalInt())));
        void* registry = reinterpret_cast<void*>(static_cast<uintptr_t>(
            static_cast<uint32_t>(SceneLoader_GetObjectRegistry(globalObject, nullptr))));
        reinterpret_cast<RegistryUnregisterFn>(CSF_AT(void**, registry, 0)[0x2C / 4])(
            registry, nullptr, self->field_0020);
        self->field_0020 = 0;
    }

    // Tail call in the original: reset base state on the same object.
    RpPrtStd_ResetBaseState(self, nullptr);
}

HOOK(0x006F2BE0, C_00862640_ReleaseHandles, "auto");
