// Commandos: Strike Force — menu mission info.
// Releases the auxiliary object associated with the menu mission info entry.
// The original function tail-jumps into 0x4369A0, whose body is reproduced inline here:
// it takes the object referenced at +0x84, and if that object holds a managed pointer at
// +0x158, it tears it down (via 0x6923C0 on the embedded object at +4) and clears the slot.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x6923C0 : releases/destroys an object whose head is a vtable pointer.
// Prototype from the packet: int __thiscall sub_6923C0(void (__thiscall ***this)(void *, int))
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x6923C0, (void* self, void* edx));

static void __fastcall CMenuMissionInfo_ReleaseStoredObject(void* self, void* /*edx*/)
{
    uint8_t* pStoredObject = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x84));
    if (!pStoredObject)
        return;

    uint32_t pManagedObject = CSF_AT(uint32_t, pStoredObject, 0x158);
    if (pManagedObject)
    {
        ReleaseObjectReference(reinterpret_cast<void*>(pManagedObject + 4), nullptr);
        CSF_AT(uint32_t, pStoredObject, 0x158) = 0;
    }
}

HOOK(0x53D4F0, CMenuMissionInfo_ReleaseStoredObject, "auto");
