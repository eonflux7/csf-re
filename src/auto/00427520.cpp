#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Tears down the auxiliary HUD panel: three child objects at +0x0C..+0x14 are
// deleted (vtable slot 0 with flag 1) and cleared, the object at +0x20 has its
// reference released (object + 4), then the objects at +0x18 and +0x1C are deleted.
static int __fastcall Hud_DestroyAuxiliaryPanel(void* self, void* /*edx*/)
{
    // Three child slots starting at +0x0C.
    for (int slotIndex = 0; slotIndex < 3; ++slotIndex)
    {
        const int slotOffset = 0x0C + slotIndex * 4;
        uint32_t childObject = CSF_AT(uint32_t, self, slotOffset);
        if (childObject)
        {
            void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childObject));
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
            CSF_AT(uint32_t, self, slotOffset) = 0;
        }
    }

    uint32_t referencedObject = CSF_AT(uint32_t, self, 0x20);
    int result = static_cast<int>(referencedObject);
    if (referencedObject)
    {
        void* referenceBase = reinterpret_cast<void*>(static_cast<uintptr_t>(referencedObject + 4));
        result = ReleaseObjectReference(referenceBase, nullptr);
    }

    uint32_t highlightObject = CSF_AT(uint32_t, self, 0x18);
    if (highlightObject)
    {
        void* highlight = reinterpret_cast<void*>(static_cast<uintptr_t>(highlightObject));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, highlight, 0)[0])(highlight, nullptr, 1);
        CSF_AT(uint32_t, self, 0x18) = 0;
    }

    uint32_t fadeObject = CSF_AT(uint32_t, self, 0x1C);
    if (fadeObject)
    {
        void* fade = reinterpret_cast<void*>(static_cast<uintptr_t>(fadeObject));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, fade, 0)[0])(fade, nullptr, 1);
        CSF_AT(uint32_t, self, 0x1C) = 0;
    }

    return result;
}

HOOK(0x00427520, Hud_DestroyAuxiliaryPanel, "auto");
