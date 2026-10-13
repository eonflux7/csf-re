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

CSF_FUNC(void*, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_PrepareMountPlacementAndStartMovement, 0x004A7240, (C_0082f9f8* self, void*));

// Resolves the object linked to a1, steps back 4 bytes into its base, follows
// the chain +0x2C -> +0x34 -> +0x310 and starts the mount placement movement on
// the C_0082f9f8 found there. Returns 0 for a null link (faithfully reads from
// address 0x2C in that case, as the original does).
static int __stdcall StartMountMovementFromLinkedObject(int a1)
{
    void* linkedRaw = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    int linkedValue = static_cast<int>(reinterpret_cast<uintptr_t>(linkedRaw));

    int baseValue = 0;
    if (linkedValue != 0)
        baseValue = linkedValue - 4;

    void* baseObj = reinterpret_cast<void*>(static_cast<uintptr_t>(baseValue));
    void* ownerObj = CSF_AT(void*, baseObj, 0x2C);
    void* placementHolder = CSF_AT(void*, ownerObj, 0x34);
    C_0082f9f8* mountPlacement = CSF_AT(C_0082f9f8*, placementHolder, 0x310);

    return C_0082f9f8_PrepareMountPlacementAndStartMovement(mountPlacement, nullptr);
}

HOOK(0x004AD530, StartMountMovementFromLinkedObject, "auto");
