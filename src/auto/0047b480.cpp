#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: 0x004DB440 CharTask_LookupByOffset (__thiscall, one stack arg, ret 4)
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

using WeaponReadyVirtualFn = int(__fastcall*)(void* self, void* edx);

// Original: 0x0047B480, __thiscall with one stack argument (ret 4).
// Asks the object held at this->field_0008 -> +0x14 -> +0x44 (virtual slot 0x90)
// whether the condition holds; if it does, runs the lookup on this + (field_000c + 60).
static int __fastcall C_00834fa8_ConditionalLookupByOffset(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    uint8_t* ownerSub = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    void* checkedObject = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerSub, 0x44));

    int result = reinterpret_cast<WeaponReadyVirtualFn>(CSF_AT(void**, checkedObject, 0)[0x90 / 4])(checkedObject, nullptr);
    if (static_cast<uint8_t>(result))
    {
        int lookupOffset = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 60);
        return CharTask_LookupByOffset(self, nullptr, lookupOffset);
    }
    return result;
}

HOOK(0x47B480, C_00834fa8_ConditionalLookupByOffset, "auto");
