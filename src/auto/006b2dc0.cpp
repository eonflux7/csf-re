#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resolves the target sub-object: the fixed one at this+0x30 when index is -1,
// otherwise the secondary entry for the index found by vSlot98. Then it calls
// a2->vtbl[6](flag) and passes the result to the sub-object's vtbl[33].
static int __fastcall C_00860288_ApplyFlagToSubObject(C_00860288* self, void* /*edx*/,
                                                      int a2, int a3, int a4)
{
    int result = a3;
    uint32_t* target = nullptr;

    if (a3 == -1) {
        target = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x30);
    } else {
        result = self->vSlot98(a3);
        if (result == -1)
            return result;

        int secondaryEntry = self->vSlot8C(static_cast<unsigned int>(result));
        void* secondaryObject = reinterpret_cast<void*>(static_cast<uintptr_t>(secondaryEntry));

        // secondaryObject->vtbl[6]()
        result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, secondaryObject, 0)[6])(
            secondaryObject, nullptr);
        target = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(result));
    }

    if (target) {
        int flagValue = (a4 == 1) ? 1 : 0;
        void** targetVtbl = CSF_AT(void**, target, 0);

        void* sourceObject = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
        // a2->vtbl[6](flagValue)
        int token = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, sourceObject, 0)[6])(
            sourceObject, nullptr, flagValue);

        // target->vtbl[33](token)
        return ((int (__fastcall*)(void*, void*, int))targetVtbl[33])(target, nullptr, token);
    }

    return result;
}

HOOK(0x6B2DC0, C_00860288_ApplyFlagToSubObject, "auto");
