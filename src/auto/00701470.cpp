#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxObjectInstances_TouchCachedEntry, 0x007013B0, (void* self, void* edx, int a2));

// The original's call to 0x7013B0 references S_007013b0's vtable per the packet.
[[maybe_unused]] static const void* const kTouchCachedEntryVtbl = CSF_VTBL(0x007013B0);

// Entry layout (16 bytes): [0] key, [1] cache flag (-1 = untouched), [2] stamp, [3] value.
static int __fastcall GfxObjectInstances_FindInstance(S_007013b0* self, void* /*edx*/, int slotIndex, int expectedKey)
{
    int result = 0;
    if (slotIndex != -1)
    {
        int* entry = reinterpret_cast<int*>(self->field_0008 + 16u * static_cast<uint32_t>(slotIndex));
        if (entry[0] == expectedKey)
        {
            int value = entry[3];
            entry[2] = static_cast<int>(self->field_0034);
            if (entry[1] != -1)
            {
                GfxObjectInstances_TouchCachedEntry(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
            }
            return value;
        }
    }
    return result;
}

HOOK(0x00701470, GfxObjectInstances_FindInstance, "auto");
