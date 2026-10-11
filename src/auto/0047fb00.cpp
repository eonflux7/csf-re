#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Dispatches to the virtual handler selected by the state index at +0x0C.
// Case k calls vtbl[0x16C/4 + k] with (this, a2), and the result is returned unchanged.
// States above 9 return the raw state value.
// The original tail-jumps with `ret 4`, so this __fastcall version (one stack arg) also pops 4.
// The original is a jump table of tail jumps to this object's state handlers. The table is NOT in block
// order: states 0..8 go to slots +0x16C, +0x174, +0x178, ... +0x190 and state 9 to +0x170 (the second block).
static int __fastcall CCharWeaponFsm_DispatchStateHandler(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    const unsigned int state = self->m_stateCode;
    switch (state)
    {
    case 0: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x16C / 4])(self, nullptr, a2);
    case 1: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x174 / 4])(self, nullptr, a2);
    case 2: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x178 / 4])(self, nullptr, a2);
    case 3: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x17C / 4])(self, nullptr, a2);
    case 4: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x180 / 4])(self, nullptr, a2);
    case 5: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x184 / 4])(self, nullptr, a2);
    case 6: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x188 / 4])(self, nullptr, a2);
    case 7: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x18C / 4])(self, nullptr, a2);
    case 8: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x190 / 4])(self, nullptr, a2);
    case 9: return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x170 / 4])(self, nullptr, a2);
    default: return static_cast<int>(state);
    }
}

HOOK(0x0047FB00, CCharWeaponFsm_DispatchStateHandler, "auto");
