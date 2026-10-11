#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));

static unsigned int __fastcall C_00840e68_UnregisterObject(C_00840e68* self, void* /*edx*/)
{
    unsigned int result = Hud_UnregisterObject(reinterpret_cast<int>(self));
    self->field_0004 = 0;
    return result;
}

HOOK(0x0051C090, C_00840e68_UnregisterObject, "auto");
