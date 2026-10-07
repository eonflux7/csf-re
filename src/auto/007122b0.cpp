#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall C_00863dd0_HasNonZeroInnerField50(
    C_00863dd0* self, void* /*edx*/)
{
    auto* innerObject = reinterpret_cast<void*>(self->field_0004);
    return CSF_AT(uint32_t, innerObject, 0x50) != 0;
}

HOOK(0x007122B0, C_00863dd0_HasNonZeroInnerField50, "auto");
