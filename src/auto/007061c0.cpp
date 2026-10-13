#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Slot +0x18 (index 6) of the notified object's vtable: __thiscall(obj, a2), callee pops its own stack arg.
using C_00863260_NotifySlot6Fn = void(__fastcall*)(void* self, void* edx, uint32_t* a2);

// Copies three dwords from the argument into this+0x0C..+0x14, notifies the object at +0x24,
// then notifies each child in the 16-byte-stride array at +0x28 (count at +0x30).
static unsigned int __fastcall C_00863260_CopyVec3AndNotifyChildren(C_00863260* self, void* /*edx*/, uint32_t* a2)
{
    CSF_AT(uint32_t, self, 0x0c) = a2[0];
    CSF_AT(uint32_t, self, 0x10) = a2[1];
    CSF_AT(uint32_t, self, 0x14) = a2[2];

    void* primary = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x24));
    void** primaryVtbl = CSF_AT(void**, primary, 0);
    reinterpret_cast<C_00863260_NotifySlot6Fn>(primaryVtbl[6])(primary, nullptr, a2);

    unsigned int result = CSF_AT(uint32_t, self, 0x30);
    unsigned int index = 0;
    CSF_AT(uint8_t, self, 0x38) = 0;

    if (result != 0) {
        uint32_t offset = 0;
        do {
            uint8_t* children = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x28));
            void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, children, offset));
            void** childVtbl = CSF_AT(void**, child, 0);
            reinterpret_cast<C_00863260_NotifySlot6Fn>(childVtbl[6])(child, nullptr, a2);

            result = CSF_AT(uint32_t, self, 0x30);
            ++index;
            offset += 16;
        } while (index < result);
    }

    CSF_AT(uint8_t, self, 0x39) = 0;
    return result;
}

HOOK(0x007061C0, C_00863260_CopyVec3AndNotifyChildren, "auto");
