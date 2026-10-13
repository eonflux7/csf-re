#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, sub_7FAFE0, 0x007FAFE0, (uint32_t* a1, uint32_t* a2));

// Copies the three dwords from the argument array into this object's
// slots at +0xDC/+0xE0/+0xE4, then returns the result of sub_7FAFE0 applied
// to the scene's child count (from the scene stored at +0x3C) and the array.
static int __fastcall C_00861120_StoreTripleAndProcessScene(C_00861120* self, void* /*edx*/, uint32_t* a2)
{
    self->field_00dc = a2[0];
    CSF_AT(uint32_t, self, 0xE0) = a2[1];
    CSF_AT(uint32_t, self, 0xE4) = a2[2];

    int childCount = Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));
    uint32_t* processed = sub_7FAFE0(
        reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(childCount)), a2);
    return static_cast<int>(reinterpret_cast<uintptr_t>(processed));
}

HOOK(0x006D6280, C_00861120_StoreTripleAndProcessScene, "auto");
