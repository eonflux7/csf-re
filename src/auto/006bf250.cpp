#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns true when the record linked from field_000c has a non-zero dword at +0x04.
static bool __fastcall C_00860700_IsLinkedRecordValueNonZero(C_00860700* self, void* /*edx*/)
{
    uint8_t* linkedRecord = reinterpret_cast<uint8_t*>(self->field_000c);
    uint32_t linkedValue = CSF_AT(uint32_t, linkedRecord, 0x4);
    return linkedValue != 0;
}

HOOK(0x006BF250, C_00860700_IsLinkedRecordValueNonZero, "auto");
