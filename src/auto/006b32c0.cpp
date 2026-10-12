#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_ApplySlotState, 0x00692200,
         (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));

static int __cdecl AiAttackTasks_UpdateSlotFromManager(int owner, S_00691860* slot)
{
    S_00691860* originalSlot = slot;
    void* manager = reinterpret_cast<uint8_t*>(owner) + 8;

    if (static_cast<uint8_t>(originalSlot->field_0014) != 0)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, manager, 0)[4])(manager, nullptr);

        S_00691860* slotCandidate = reinterpret_cast<S_00691860*>(
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, manager, 0)[5])(manager, nullptr));

        bool resetFlag = false;
        if (CSF_AT(uint8_t, originalSlot, 0) == 0)
        {
            void* handler = reinterpret_cast<void*>(originalSlot->field_0004);
            resetFlag = !((bool(__fastcall*)(void*, void*, S_00691860**, S_00691860*))
                CSF_AT(void**, handler, 0)[35])(
                    handler, nullptr, &slotCandidate, originalSlot);
        }

        void* receiver = reinterpret_cast<void*>(originalSlot->field_000c);
        S_00691860* resolvedSlot = slotCandidate;
        CSF_AT(uint8_t, originalSlot, 0) = resetFlag;

        ((void(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, receiver, 0)[2])(receiver, nullptr, resolvedSlot);
    }
    else
    {
        S_00691860* slotCandidate = originalSlot;

        bool resetFlag = false;
        if (CSF_AT(uint8_t, originalSlot, 0) == 0)
        {
            void* handler = reinterpret_cast<void*>(originalSlot->field_0004);
            resetFlag = !((bool(__fastcall*)(void*, void*, S_00691860**, S_00691860*))
                CSF_AT(void**, handler, 0)[35])(
                    handler, nullptr, &slotCandidate, originalSlot);
        }

        CSF_AT(uint8_t, originalSlot, 0) = resetFlag;

        int managerState = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, manager, 0)[5])(manager, nullptr);

        AiAttackTasks_ApplySlotState(
            CSF_AT(void*, originalSlot, 0x08),
            nullptr,
            reinterpret_cast<unsigned int>(slotCandidate),
            managerState,
            reinterpret_cast<int>(originalSlot),
            0);
    }

    return ((int(__fastcall*)(void*, void*, S_00691860*))
        CSF_AT(void**, manager, 0)[2])(manager, nullptr, originalSlot);
}

HOOK(0x006B32C0, AiAttackTasks_UpdateSlotFromManager, "auto");
