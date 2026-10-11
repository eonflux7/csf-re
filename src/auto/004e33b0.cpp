#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_493270, 0x00493270, (void* self, void* edx));

static int __fastcall C_0083e248_ResetStateIfPairMatches(C_0083e248* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(a2));
    const uint32_t selfValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));

    if (a2[1] == selfValue && a2[0] != selfValue)
    {
        if (self->field_0018 == 3)
        {
            sub_493270(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)), nullptr);

            uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c));
            uint8_t* inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x34)));
            CSF_AT(uint32_t, inner, 0x18) |= 2u;

            uint8_t* virtualOwner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c)) + 4;
            uint8_t flag = 0;
            result = ((int(__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, virtualOwner, 0)[0x50 / 4])(
                virtualOwner, nullptr, &flag);
        }
        self->field_0018 = 0;
    }
    return result;
}

HOOK(0x004E33B0, C_0083e248_ResetStateIfPairMatches, "auto");
