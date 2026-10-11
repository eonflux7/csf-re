#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00861120_SetBlockingType(C_00861120* self, void* /*edx*/, int blockingType)
{
    self->field_00f4 = static_cast<uint32_t>(blockingType);
    return blockingType;
}

HOOK(0x00732C80, C_00861120_SetBlockingType, "auto");
