#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall C_00833708_AreAllRecordStatesZeroOrOne(
    C_00833708* self, void* /*edx*/)
{
    uint32_t remaining = self->field_0014;
    char result = 1;

    if (remaining != 0) {
        const uint8_t* record =
            reinterpret_cast<const uint8_t*>(self->field_000c);

        do {
            const uint32_t state = CSF_AT(uint32_t, record, 0x04);
            result = static_cast<char>(result & (state < 2u));
            record += 0x60;
            --remaining;
        } while (remaining != 0);
    }

    return result;
}

HOOK(0x00489C00, C_00833708_AreAllRecordStatesZeroOrOne, "auto");
