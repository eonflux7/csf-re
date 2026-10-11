#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00446450_GrowStorage, 0x00446450,
         (void* self, void* edx, int a2));

static uint32_t* __fastcall S_00446450_AppendRecord(
    S_00446450* self, void* /*edx*/, uint32_t* source, int growth)
{
    if (self->field_0008 >= self->field_0004)
    {
        int amount = growth;
        if (!amount)
            amount = static_cast<int>(self->field_000c);
        S_00446450_GrowStorage(self, nullptr, amount);
    }

    const uint32_t index = self->field_0008;
    uint32_t* destination =
        reinterpret_cast<uint32_t*>(self->field_0000 + 40 * index);
    self->field_0008 = index + 1;

    destination[0] = source[0];
    destination[1] = source[1];
    destination[3] = source[3];
    destination[4] = source[4];
    destination[5] = source[5];
    destination[6] = source[6];
    destination[7] = source[7];
    destination[8] = source[8];
    destination[9] = source[9];
    return destination;
}

HOOK(0x00446C80, S_00446450_AppendRecord, "auto");
