#include <cstdint>
#include "globals.h"
#include "hook.h"

// Clump/object identity check at +12 of a child, child list at this+0x14, count at this+0x1c.
static int __fastcall GfxScene_FindChildByIdentifier(void* self, void* /*edx*/, int identifier)
{
    uint32_t index = 0;

    if (CSF_AT(uint32_t, self, 0x1c) == 0)
        return 0;

    while (true)
    {
        void* element = CSF_AT(void**, self, 0x14)[index];

        // vtable slot 6 (offset 24) returns the child's identifier source object
        void* child = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, element, 0)[6])(element, nullptr);

        if (CSF_AT(int, child, 12) == identifier)
            return static_cast<int>(reinterpret_cast<uintptr_t>(CSF_AT(void**, self, 0x14)[index]));

        ++index;
        if (index >= CSF_AT(uint32_t, self, 0x1c))
            return 0;
    }
}

HOOK(0x6CB820, GfxScene_FindChildByIdentifier, "auto");
