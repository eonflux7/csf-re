#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: C_008670f0::SetEntry (__thiscall, ret 8), bound as __fastcall.
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* /*edx*/, int key, int value));

// Forwards `a2` as the value for key 3 on the object stored at this->field_0008.
static int __fastcall C_00861bd8_SetEntryKey3(C_00861bd8* self, void* /*edx*/, int a2)
{
    return C_008670f0_SetEntry(reinterpret_cast<C_008670f0*>(self->field_0008), nullptr, 3, a2);
}

HOOK(0x006E5130, C_00861bd8_SetEntryKey3, "auto");
