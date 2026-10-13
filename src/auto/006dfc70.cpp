#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Removes a registered object from this container's slot array.
// Searches field_0014[0..field_001c) for a2; if found, notifies the object
// (vtbl slot 9, then vtbl slot 1 with arg 1), decrements the count and clears the slot.
static void __fastcall C_00861988_RemoveRegisteredObject(C_00861988* self, void* /*edx*/, int a2)
{
    const uint32_t slotCount = self->field_001c;
    uint32_t foundIndex = 0;

    if (slotCount == 0) {
        return;
    }

    uint32_t* slotPtr = reinterpret_cast<uint32_t*>(self->field_0014);
    const uint32_t target = static_cast<uint32_t>(a2);
    for (; *slotPtr != target; ++slotPtr) {
        if (++foundIndex >= slotCount) {
            return;
        }
    }

    if (foundIndex != 0xFFFFFFFFu) {
        void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
        // slot 36 (+0x24) / 4 = 9
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[9])(obj, nullptr);
        // slot 4 / 4 = 1
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[1])(obj, nullptr, 1);

        self->field_0050--;
        uint32_t* entries = reinterpret_cast<uint32_t*>(self->field_0014);
        entries[foundIndex] = 0;
    }
}

HOOK(0x6DFC70, C_00861988_RemoveRegisteredObject, "auto");
