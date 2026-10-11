#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_48C720, 0x0048C720, (void* self, void* edx, int a2, int a3));

// Returns true when the attack task's target check passes. Returns false only when the
// sub-object at +0x2B8 does not accept the target AND the object's virtual slot 43 accepts it.
static bool __fastcall AiAttackTasks_CheckTargetValid(S_0047ae10* self, void* /*edx*/)
{
    (void)CSF_VTBL(0x0047AE10);

    char* taskTargetNameBuffer = AiCore_GetTaskTargetNameBuffer(self, nullptr);
    bool result = true;
    if (!sub_48C720(&self->pad_0020[664], nullptr,
                    (int)(self->field_0014 + 28),
                    reinterpret_cast<int>(taskTargetNameBuffer + 16)))
    {
        char* nameBuffer = AiCore_GetTaskTargetNameBuffer(self, nullptr);
        using SlotFn = bool(__fastcall*)(void*, void*, char*);
        SlotFn slotFn = reinterpret_cast<SlotFn>(CSF_AT(void**, self, 0)[0xac / 4]);
        if (slotFn(self, nullptr, nameBuffer + 16))
            return false;
    }
    return result;
}

HOOK(0x0047BBA0, AiAttackTasks_CheckTargetValid, "auto");
