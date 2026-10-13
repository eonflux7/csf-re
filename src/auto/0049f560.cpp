#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, sub_49F330, 0x0049F330, (void* self, void* edx, int a2));

// Resets the pending flag at +0x1A0 and, if the new value a2 is above the stored
// value at +0x19C, forwards the difference to the sub-object at +0x198.
static void* __fastcall C_00833908_SetSceneCallback(void* self, void* /*edx*/, void* a2)
{
    if (CSF_AT(uint32_t, self, 0x1A0) != 0)
        CSF_AT(uint32_t, self, 0x1A0) = 0;

    uint32_t storedValue = CSF_AT(uint32_t, self, 0x19C);
    uint32_t newValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a2));

    if (newValue > storedValue)
    {
        void* subObject = static_cast<uint8_t*>(self) + 0x198;
        int delta = static_cast<int>(newValue - storedValue);
        return sub_49F330(subObject, nullptr, delta);
    }
    return a2;
}

HOOK(0x0049F560, C_00833908_SetSceneCallback, "auto");
