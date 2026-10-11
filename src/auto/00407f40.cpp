#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"

// Returns true when the 28-byte record at `index` in the table pointed to by
// this->field_000C has a non-zero first dword.
static bool __fastcall IsRecordSlotActive(void* self, void* /*edx*/, int index)
{
    uint8_t* table = CSF_AT(uint8_t*, self, 0x0C);
    uint32_t firstDword = CSF_AT(uint32_t, table, 28 * index);
    return firstDword != 0;
}

HOOK(0x00407F40, IsRecordSlotActive, "auto");
