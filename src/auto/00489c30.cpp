#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_00833708_CanUseSlot(C_00833708* self, void* /*edx*/, int slot)
{
    const uint32_t offset = 96u * static_cast<uint32_t>(slot);
    const auto* entry = reinterpret_cast<const uint8_t*>(self->field_000c + offset);
    return CSF_AT(uint32_t, entry, 0x04) == 1;
}

HOOK(0x00489C30, C_00833708_CanUseSlot, "auto");
