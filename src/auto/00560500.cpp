#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __cdecl MenuMpSessions_SetControlState(
    std::uint32_t controlAddress, int state)
{
    if (!controlAddress)
        return 0;

    void* control = reinterpret_cast<void*>(controlAddress);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, state);
}

static int __fastcall CMenuMpSessions_ApplySessionControlStates(
    CMenuMpSessions* self, void* /*edx*/)
{
    // Read each control only when its turn is reached: callbacks may change fields.
    MenuMpSessions_SetControlState(self->field_0090, 1);
    MenuMpSessions_SetControlState(self->field_0078, 1);
    MenuMpSessions_SetControlState(self->field_0094, 1);
    MenuMpSessions_SetControlState(self->field_0024, 1);
    MenuMpSessions_SetControlState(self->field_0098, 1);
    MenuMpSessions_SetControlState(self->field_0028, 1);
    MenuMpSessions_SetControlState(self->field_009c, 1);
    MenuMpSessions_SetControlState(self->field_002c, 1);
    MenuMpSessions_SetControlState(self->field_0030, 1);
    MenuMpSessions_SetControlState(self->field_00a0, 1);

    MenuMpSessions_SetControlState(self->field_00a8, 0);
    MenuMpSessions_SetControlState(self->field_007c, 0);
    MenuMpSessions_SetControlState(self->field_0088, 0);
    MenuMpSessions_SetControlState(self->field_0080, 0);
    MenuMpSessions_SetControlState(self->field_008c, 0);
    return MenuMpSessions_SetControlState(self->field_0084, 0);
}

HOOK(0x00560500, CMenuMpSessions_ApplySessionControlStates, "auto");
