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

CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(char, __fastcall, AiAttackTasks_SetActionFlag, 0x0048C9D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

// Returns *(*(self+8)+0x14)+0x204 value: the character task used by the attack task calls.
static void* AutonomoGetCharTask(void* self)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    void* ai = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    return reinterpret_cast<void*>(CSF_AT(uint32_t, ai, 0x204));
}

static int __fastcall TAutonomo_StartAttackWithTargetHandle(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x14) = 0;

    TaskCharAction_48CFE0(AutonomoGetCharTask(self), nullptr,
                          static_cast<int>(reinterpret_cast<uintptr_t>(self)), 5, 1);

    AiAttackTasks_SetActionFlag(AutonomoGetCharTask(self), nullptr, 1);

    alignas(8) uint8_t handleBytes[12];
    uint32_t* handle = reinterpret_cast<uint32_t*>(handleBytes);
    void* handleVtbl = CSF_VTBL(0x0082D448);
    handle[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handleVtbl));
    handle[1] = 0xFFFFFFFFu;
    handle[2] = 0xFFFFFFFFu;

    // Referenced by the original at the CopyTargetHandle call site (S_0048c9f0 vtable).
    (void)CSF_VTBL(0x0048C9F0);

    return AiAttackTasks_CopyTargetHandle(AutonomoGetCharTask(self), nullptr,
                                          static_cast<int>(reinterpret_cast<uintptr_t>(handle)));
}

HOOK(0x4D88E0, TAutonomo_StartAttackWithTargetHandle, "auto");
