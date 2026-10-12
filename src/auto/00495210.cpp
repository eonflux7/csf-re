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

CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveActiveItemHandle, 0x00485B70, (CCharActionFsm* self, void*));
CSF_FUNC(char, __fastcall, CCharActionFsm_CanUseItem, 0x00487400, (CCharActionFsm* self, void* , uint32_t* item));

// __thiscall(this, a2) with one stack argument, ret 4.
static char __fastcall CharActor_TryLatchItemAction(void* self, void* /*edx*/, uint8_t* a2)
{
    CCharActionFsm* fsm = CSF_AT(CCharActionFsm*, self, 0x40);
    int activeHandle = CCharActionFsm_ResolveActiveItemHandle(fsm, nullptr);
    char canUse = CCharActionFsm_CanUseItem(fsm, nullptr,
        reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(activeHandle)));
    if (canUse)
    {
        CSF_AT(uint8_t, self, 0x33C) = *a2;
        CSF_AT(uint32_t, self, 0x2E0) |= 0x100u;
        return static_cast<char>(reinterpret_cast<uintptr_t>(a2));
    }
    return canUse;
}

HOOK(0x495210, CharActor_TryLatchItemAction, "auto");
