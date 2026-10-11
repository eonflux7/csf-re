#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, sub_520AE0, 0x00520AE0, (void* self, void* edx, int a2));

static std::uint32_t* __fastcall S_00520ae0_AppendParticle(
    S_00520ae0* self, void* /*edx*/, std::uint32_t* source, int growth)
{
    if (self->field_0008 >= self->field_0004)
    {
        int amount = growth;
        if (!amount)
            amount = static_cast<int>(self->field_000c);
        sub_520AE0(self, nullptr, amount);
    }

    const std::uint32_t index = self->field_0008;
    auto* destination = reinterpret_cast<std::uint32_t*>(
        self->field_0000 + 36u * index);
    self->field_0008 = index + 1;

    destination[0] = source[0];
    destination[2] = source[2];
    destination[3] = source[3];
    destination[4] = source[4];
    destination[5] = source[5];
    destination[6] = source[6];
    destination[7] = source[7];
    destination[8] = source[8];
    return destination;
}

HOOK(0x00520D80, S_00520ae0_AppendParticle, "auto");
