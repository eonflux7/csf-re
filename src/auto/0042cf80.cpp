#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Releases the two referenced HUD sub-objects (this+4, this+8) and the owned
// object at this+0 (virtual slot 1 = release/destroy with flag 1), clearing each slot.
// Returns the value of the last step: the field-2 value/release result, or the
// virtual call result when the owner pointer was set.
static int __fastcall HudPanel_ReleaseReferences(int* self, void* /*edx*/)
{
    uint32_t widgetRefA = CSF_AT(uint32_t, self, 0x4);
    if (widgetRefA != 0)
    {
        void* widgetObjA = reinterpret_cast<void*>(static_cast<uintptr_t>(widgetRefA) + 4);
        ReleaseObjectReference(widgetObjA, nullptr);
        CSF_AT(uint32_t, self, 0x4) = 0;
    }

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x8));
    uint32_t widgetRefB = CSF_AT(uint32_t, self, 0x8);
    if (widgetRefB != 0)
    {
        void* widgetObjB = reinterpret_cast<void*>(static_cast<uintptr_t>(widgetRefB) + 4);
        result = ReleaseObjectReference(widgetObjB, nullptr);
        CSF_AT(uint32_t, self, 0x8) = 0;
    }

    uint32_t owner = CSF_AT(uint32_t, self, 0x0);
    if (owner != 0)
    {
        void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(owner));
        // vtable slot 1 (byte offset 4), called __thiscall with one argument: 1
        result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, ownerObj, 0)[1]))(ownerObj, nullptr, 1);
        CSF_AT(uint32_t, self, 0x0) = 0;
    }
    return result;
}

HOOK(0x0042CF80, HudPanel_ReleaseReferences, "auto");
