#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Reads the byte flag at offset 0x1b3.
static char __fastcall C_00863d80_GetField01b3(C_00863d80* self, void* /*edx*/)
{
    return static_cast<char>(self->field_01b3);
}

HOOK(0x00711D60, C_00863d80_GetField01b3, "auto");
