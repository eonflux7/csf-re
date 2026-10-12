#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// unsigned int __thiscall sub_443290(void* self, float a2)  -> ret 4
CSF_FUNC(unsigned int, __fastcall, CommandoAnims_AdvanceAuxiliaryState, 0x00443290, (void* self, void* edx, float a2));
// int __thiscall sub_442F90(void* self)                     -> ret 0
CSF_FUNC(int, __fastcall, CommandoAnims_EvaluateAuxiliaryState, 0x00442F90, (void* self, void* edx));

// The original references S_00442f90's vtable at the call site; it is not stored by this function.
[[maybe_unused]] static const auto kS00442fVtableRef = CSF_VTBL(0x00442F90);

// 0x00443350: advances the auxiliary animation by the incoming float argument
// (via 0x443290, this in ecx), then re-evaluates the state (via 0x442F90) and returns its result.
static int __fastcall CommandoAnims_UpdateAuxiliaryAnimation(S_00442f90* self, void* /*edx*/, float a2)
{
    CommandoAnims_AdvanceAuxiliaryState(self, nullptr, a2);
    return CommandoAnims_EvaluateAuxiliaryState(self, nullptr);
}

HOOK(0x00443350, CommandoAnims_UpdateAuxiliaryAnimation, "auto");
