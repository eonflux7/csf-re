#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x00498F50: __fastcall-style callee with two stack args (ret 8).
CSF_FUNC(int, __fastcall, CharFsm_PostStateRequest, 0x00498F50, (void* self, void* edx, int a2, char a3));

// Original 0x004802C0: __thiscall callee, no stack args (ret 0).
CSF_FUNC(int, __fastcall, CCharWeaponFsm_RefreshActionState, 0x004802C0, (void* self, void* edx));

static int __fastcall CCharActionFsm_ResetItemStateAndDispatch(CCharActionFsm* self, void* /*edx*/)
{
    // Original: mov ecx, [esi+8]; push 1; push 0x80; call 0x498F50
    CharFsm_PostStateRequest(reinterpret_cast<void*>(self->field_0008), nullptr, 0x80, 1);

    // Original: mov ecx, esi; jmp 0x4802C0  (tail jump, result returned unchanged)
    return CCharWeaponFsm_RefreshActionState(self, nullptr);
}

HOOK(0x004823A0, CCharActionFsm_ResetItemStateAndDispatch, "auto");
