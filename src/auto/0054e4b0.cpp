#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_SerializeValue(
    C_008443c8* self, void* /*edx*/, uint32_t* value, uint8_t* context)
{
    uint32_t serializedValue = reinterpret_cast<uint32_t>(context);
    if (CSF_AT(uint8_t, context, 0x14))
        serializedValue = *value;

    self->vSlot48(&serializedValue, reinterpret_cast<int>(context));

    if (!CSF_AT(uint8_t, context, 0x14))
        *value = serializedValue;

    return CSF_AT(uint8_t, context, 0x0) == 0;
}

HOOK(0x0054E4B0, C_008443c8_SerializeValue, "auto");
