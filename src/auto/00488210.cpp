#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharWeaponFsm_FindStateIndex, 0x00485A20,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetItemAction, 0x00487260,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0,
         (void* self, void*));

static int __fastcall CCharActionFsm_UpdateItemActions(
    CCharActionFsm* self, void* /*edx*/, int updateArgument)
{
    void* currentAction = reinterpret_cast<void*>(self->field_009c);
    if (currentAction &&
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, currentAction, 0)[0x4C / 4])(
                currentAction, nullptr) == 10)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, currentAction, 0)[0x30 / 4])(
                currentAction, nullptr, updateArgument);
    }

    int result = static_cast<int>(self->field_0008);
    void* character = reinterpret_cast<void*>(self->field_0008);
    if (CSF_AT(uint8_t, character, 0x228))
    {
        if ((CSF_AT(uint8_t, character, 0x1F0) & 2) != 0)
        {
            int stateIndex = CCharWeaponFsm_FindStateIndex(self, nullptr, 22);
            if (stateIndex != -1)
            {
                void* action = reinterpret_cast<void*>(
                    reinterpret_cast<uint32_t*>(self->field_0078)[stateIndex]);
                if (action)
                {
                    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, action, 0)[0x30 / 4])(
                            action, nullptr, updateArgument);
                }
            }
        }

        result = CCharWeaponFsm_FindStateIndex(self, nullptr, 28);
        if (result != -1)
        {
            uint32_t* actionSlot =
                reinterpret_cast<uint32_t*>(self->field_0078) + result;
            void* action = reinterpret_cast<void*>(*actionSlot);
            result = static_cast<int>(reinterpret_cast<uintptr_t>(actionSlot));

            if (CSF_AT(uint32_t, action, 0x80))
            {
                if (action)
                {
                    result = C_004950e0_GetField398(
                        reinterpret_cast<void*>(self->field_0008), nullptr);
                    if (result)
                    {
                        void* itemAction = reinterpret_cast<void*>(
                            CCharActionFsm_GetItemAction(self, nullptr));
                        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, itemAction, 0)[0x30 / 4])(
                                itemAction, nullptr, updateArgument);
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x00488210, CCharActionFsm_UpdateItemActions, "auto");
