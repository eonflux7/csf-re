#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082f9f8_StartCharMovement, 0x0047B140, (C_0082f9f8* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Starts character movement using the field stored at +0x714 of the object
// linked to `owner`; when no linked object exists, falls back to the dword at
// absolute address 0x718 (read through a null base, as the original does).
static int __stdcall Orden_StartMovementOnLinkedObject(int owner)
{
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(owner), nullptr);
    if (linkedObjectValue != 0)
    {
        void* linkedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(linkedObjectValue)));
        uint32_t charValue = CSF_AT(uint32_t, linkedObject, 0x714);
        return C_0082f9f8_StartCharMovement(reinterpret_cast<C_0082f9f8*>(static_cast<uintptr_t>(charValue)), nullptr);
    }

    void* nullBase = nullptr;
    uint32_t fallbackValue = CSF_AT(uint32_t, nullBase, 0x718);
    return C_0082f9f8_StartCharMovement(reinterpret_cast<C_0082f9f8*>(static_cast<uintptr_t>(fallbackValue)), nullptr);
}

HOOK(0x004ACCE0, Orden_StartMovementOnLinkedObject, "auto");
