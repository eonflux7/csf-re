#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_6D1D30 (__thiscall, one stack arg, ret 4).
CSF_FUNC(char, __fastcall, sub_6D1D30, 0x006D1D30, (void* self, void* edx, void* a2));

// Replaces 0x006D2000 (C_00860e68::__thiscall, ret 4).
// Calls sub_6D1D30 only when a2 is not the object's stored current item and a2's
// virtual slot 0x1C reports the value 3.
static void __fastcall C_00860e68_AddLightIfNotCurrent(C_00860e68* self, void* /*edx*/, C_0085e7b8* a2)
{
    if (reinterpret_cast<C_0085e7b8*>(self->field_0084) != a2)
    {
        // a2->__vftable[0x1C / 4] called with a2 as this.
        int kind = ((int(__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[0x1C / 4])(a2, nullptr);
        if (kind == 3)
        {
            sub_6D1D30(self, nullptr, a2);
        }
    }
}

HOOK(0x006D2000, C_00860e68_AddLightIfNotCurrent, "auto");
