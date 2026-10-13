#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004DC150, __fastcall-style callee with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// TOrden::<method> (__thiscall, ret 4): ecx = this, [esp+4] = a2.
// Forwards a2 as the callee's `this`, and the address of this->+4 as its stack argument.
static int __fastcall TOrden_ApplyOrderState(TOrden* self, void* /*edx*/, TAiState* a2)
{
    uint8_t* subObjectAddress = reinterpret_cast<uint8_t*>(self) + 4;
    return CoverAttack_SetState_4DC150(a2, nullptr,
                                       static_cast<int>(reinterpret_cast<uintptr_t>(subObjectAddress)));
}

HOOK(0x004AD210, TOrden_ApplyOrderState, "auto");
