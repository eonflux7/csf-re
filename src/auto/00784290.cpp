#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is a pointer
// to a weapon-table entry field that is passed through untouched.
CSF_FUNC(char*, __fastcall, WeaponDefs_ParseEntryField, 0x007840E0, (void* self, void* edx, uint32_t* a2));

// Forwards a2 to WeaponDefs_ParseEntryField (with a1 as the this pointer) when a1
// is non-null; otherwise nothing is done. The original returned an uninitialized
// eax in that case; this returns nullptr instead.
static char* __stdcall WeaponDefs_ForwardEntryIfNonNull(char* a1, uint32_t* a2)
{
    if (a1)
        return WeaponDefs_ParseEntryField(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x00784290, WeaponDefs_ForwardEntryIfNonNull, "auto");
