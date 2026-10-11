#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, RefreshDoorState, 0x00490720, (void* self, void* edx));

static unsigned int __fastcall C_00833908_SetDoorStateFlags(
    C_00833908* self, void* /*edx*/, uint32_t state, char enabled)
{
    self->field_016c = state;
    uint32_t flags = self->field_0170;
    if (enabled)
        flags |= 0x100;
    else
        flags &= 0xFFFFFEFF;
    self->field_0170 = flags;

    unsigned int result = RefreshDoorState(self, nullptr);
    auto* child = reinterpret_cast<C_0082dd78*>(self->field_004c);
    if (child)
        return reinterpret_cast<unsigned int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, child, 0)[0x38 / 4])(child, nullptr);
    return result;
}

HOOK(0x00490D20, C_00833908_SetDoorStateFlags, "auto");
