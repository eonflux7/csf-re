#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Resolves the referenced object of the primary slot (field_0008) if it is valid and
// marked active (byte +0x91). Otherwise falls back to the object referenced by the
// entry selected by field_00b0 in the table at field_00a0 (object's +0x38 pointer).
static int __fastcall C_00860bb8_ResolveReferencedObject(C_00860bb8* self, void* /*edx*/)
{
    int result = 0;

    uint32_t primaryHandle = self->field_0008;
    if (primaryHandle != 0) {
        C_0085ec18* primaryObject = reinterpret_cast<C_0085ec18*>(static_cast<uintptr_t>(primaryHandle));
        if (CSF_AT(uint8_t, primaryObject, 0x91) != 0) {
            result = static_cast<int>(reinterpret_cast<uintptr_t>(
                GetReferencedObject(primaryObject, nullptr)));
            if (result != 0) {
                return result;
            }
        }
    }

    uint32_t slotIndex = self->field_00b0;
    if (slotIndex != 0xFFFFFFFFu) {
        uint32_t* slotTable = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_00a0));
        uint32_t slotEntry = slotTable[slotIndex];
        uint32_t referencedHandle = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(slotEntry)), 0x38);
        return static_cast<int>(reinterpret_cast<uintptr_t>(
            GetReferencedObject(reinterpret_cast<void*>(static_cast<uintptr_t>(referencedHandle)), nullptr)));
    }

    return result;
}

HOOK(0x006CBEF0, C_00860bb8_ResolveReferencedObject, "auto");
