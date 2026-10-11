#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_ApplySlotState, 0x00692200,
         (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));

static int __cdecl AiAttackTasks_ResetSlot(int owner, S_00691860* slot)
{
    S_00691860* originalSlot = slot;
    void* slotManager = reinterpret_cast<uint8_t*>(owner) + 4;

    if (static_cast<uint8_t>(originalSlot->field_0014) != 0)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, slotManager, 0)[4])(slotManager, nullptr);

        slot = reinterpret_cast<S_00691860*>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, slotManager, 0)[5])(slotManager, nullptr));

        bool resetFlag = false;
        if (CSF_AT(uint8_t, originalSlot, 0) == 0)
        {
            void* handler = reinterpret_cast<void*>(originalSlot->field_0004);
            resetFlag = !((bool(__fastcall*)(void*, void*, S_00691860**, S_00691860*))
                CSF_AT(void**, handler, 0)[35])(
                    handler, nullptr, &slot, originalSlot);
        }

        void* receiver = reinterpret_cast<void*>(originalSlot->field_000c);
        S_00691860* resolvedSlot = slot;
        CSF_AT(uint8_t, originalSlot, 0) = resetFlag;

        ((void(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, receiver, 0)[2])(receiver, nullptr, resolvedSlot);
    }
    else
    {
        bool resetFlag = false;
        if (CSF_AT(uint8_t, originalSlot, 0) == 0)
        {
            void* handler = reinterpret_cast<void*>(originalSlot->field_0004);
            resetFlag = !((bool(__fastcall*)(void*, void*, S_00691860**, S_00691860*))
                CSF_AT(void**, handler, 0)[35])(
                    handler, nullptr, &slot, originalSlot);
        }

        CSF_AT(uint8_t, originalSlot, 0) = resetFlag;

        int managerState = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, slotManager, 0)[5])(slotManager, nullptr);

        AiAttackTasks_ApplySlotState(
            CSF_AT(void*, originalSlot, 0x08),
            nullptr,
            reinterpret_cast<unsigned int>(slot),
            managerState,
            reinterpret_cast<int>(originalSlot),
            0);
    }

    return ((int(__fastcall*)(void*, void*, S_00691860*))
        CSF_AT(void**, slotManager, 0)[2])(
            slotManager, nullptr, originalSlot);
}

HOOK(0x0047BCB0, AiAttackTasks_ResetSlot, "auto");
