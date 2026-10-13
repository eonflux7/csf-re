#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_004f1210_GrowStorage, 0x004F1210,
         (void* self, void* edx, int a2));

static uint32_t* __fastcall S_004f1210_AppendRecord(
    S_004f1210* self, void* /*edx*/, uint32_t* source, int growth)
{
    // The original has a call-site reference to S_004f1210's vtable address.
    // No store uses it, so this only keeps the reference present.
    [[maybe_unused]] const auto classVtable = CSF_VTBL(0x004F1210);

    if (self->field_0008 >= self->field_0004)
    {
        int amount = growth;
        if (!amount)
            amount = static_cast<int>(self->field_000c);
        S_004f1210_GrowStorage(self, nullptr, amount);
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

HOOK(0x004F17B0, S_004f1210_AppendRecord, "auto");
