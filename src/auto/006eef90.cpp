#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicDestroy, 0x005821A0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveAtomic, 0x0058DE00, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpAtomicGetWorld, 0x0058DE40, (int a1));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));

// Releases the PTank atomic (removing it from its world first), the owned object
// at +0x34, the embedded gfx state at +0x08, then rebuilds the base-class state.
static void __fastcall C_008620c8_ReleasePTankAtomic(C_008620c8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862110);

    if (self->field_0004)
    {
        // Embedded object at +0x08 starts with its own vtable pointer; slot 0x70 -> 28.
        void** embeddedVtable = reinterpret_cast<void**>(self->field_0008);
        bool hasEmbeddedWork = reinterpret_cast<bool (__fastcall*)(void*, void*)>(embeddedVtable[28])(
            &self->field_0008, nullptr);
        if (hasEmbeddedWork)
        {
            // slot 0x68 -> 26, argument 0
            void** embeddedVtableAgain = reinterpret_cast<void**>(self->field_0008);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(embeddedVtableAgain[26])(
                &self->field_0008, nullptr, 0);
        }

        int world = RpAtomicGetWorld(static_cast<int>(self->field_0004));
        if (world)
        {
            RpWorldRemoveAtomic(world, static_cast<int>(self->field_0004));
        }

        RpPTankAtomicDestroy(reinterpret_cast<uint32_t*>(self->field_0004));
        self->field_0004 = 0;
    }

    uint32_t ownedValue = self->field_0034;
    if (ownedValue)
    {
        void* ownedObject = reinterpret_cast<void*>(ownedValue);
        // slot 4 -> 1, argument 1
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, ownedObject, 0)[1])(
            ownedObject, nullptr, 1);
        self->field_0034 = 0;
    }

    GfxObject_DestroyEmbeddedState(reinterpret_cast<C_00860630*>(&self->field_0008), nullptr);

    self->vtbl = CSF_VTBL(0x008620C8);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006EEF90, C_008620c8_ReleasePTankAtomic, "auto");
