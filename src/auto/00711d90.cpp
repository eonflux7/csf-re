#include "types/auto_classes.h"
#include "hook.h"

// Sets the byte flag at offset 0x1B4 to 1.
static void __fastcall C_00863d80_SetFlag01b4(C_00863d80* self, void* /*edx*/)
{
    self->field_01b4 = 1;
}

HOOK(0x711D90, C_00863d80_SetFlag01b4, "auto");
