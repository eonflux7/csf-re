#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Dispatches to the virtual handler selected by the state index at +0x0C.
// Case k tail-jumps through vtbl[0x16C/4 + k] with ecx = this; the caller's stack arguments (a2) and the return
// address are left untouched, so the target pops what it pops and returns to the caller directly.
// States above 9 return the raw state value with `ret 4`.
// The original is a jump table of tail jumps to this object's state handlers. The table is NOT in block
// order: states 0..8 go to slots +0x16C, +0x174, +0x178, ... +0x190 and state 9 to +0x170 (the second block).
// Original (0047fb03..0047fb15): mov eax,[ecx+0xc]; cmp eax,9; ja default; jmp [eax*4+table];
// each block: mov eax,[ecx]; pop ebp; jmp [eax+slot]. Default (0047fb6c): pop ebp; ret 4.
static hook::Tail __fastcall CCharWeaponFsm_DispatchStateHandler(void* self, const uint32_t* args)
{
    (void)args;
    CCharWeaponFsm* obj = static_cast<CCharWeaponFsm*>(self);
    const unsigned int state = obj->m_stateCode;
    switch (state)
    {
    case 0: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x16C / 4]);
    case 1: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x174 / 4]);
    case 2: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x178 / 4]);
    case 3: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x17C / 4]);
    case 4: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x180 / 4]);
    case 5: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x184 / 4]);
    case 6: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x188 / 4]);
    case 7: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x18C / 4]);
    case 8: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x190 / 4]);
    case 9: return hook::tail_jump(obj, CSF_AT(void**, obj, 0)[0x170 / 4]);
    default: return hook::tail_ret(state, 4);
    }
}

HOOK_TAIL(0x0047FB00, CCharWeaponFsm_DispatchStateHandler, "auto");
