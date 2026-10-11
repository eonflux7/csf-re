#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CharActionFsm_CopyParamsFromSource, 0x0047FAB0, (void* self, void* , int a2));

static int __fastcall AiAttackTasks_SubmitAttackParams(
    void* self, void* /*edx*/, void* source, char alternateAttack)
{
    void* character = CSF_AT(void*, self, 0x14);
    void* actionFsm = CSF_AT(void*, character, 0x44);

    alignas(8) uint8_t paramsBytes[0x24];
    CSF_AT(uint8_t, paramsBytes, 0x20) = 1;
    CSF_AT(int, paramsBytes, 0x00) = 1;
    CSF_AT(uint8_t, paramsBytes, 0x22) = 1;
    CSF_AT(void**, paramsBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, paramsBytes, 0x14) = 0;
    CSF_AT(int, paramsBytes, 0x18) = 0;
    CSF_AT(int, paramsBytes, 0x1C) = 0;
    CSF_AT(uint8_t, paramsBytes, 0x21) = 0;
    CSF_AT(int, paramsBytes, 0x04) = (alternateAttack != 0) + 1;

    const uint32_t sourceFirst = CSF_AT(uint32_t, source, 0x04);
    const uint32_t sourceSecond = CSF_AT(uint32_t, source, 0x08);
    CSF_AT(uint32_t, paramsBytes, 0x10) = sourceSecond;
    CSF_AT(uint32_t, paramsBytes, 0x0C) = sourceFirst;

    void* characterState = CSF_AT(void*, character, 0x3C);
    CSF_AT(uint8_t, paramsBytes, 0x20) =
        CSF_AT(uint8_t, characterState, 0x4D);

    const int actionType =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, actionFsm, 0)[0xF4 / 4])(
                actionFsm, nullptr);

    if (actionType == 3)
    {
        return CharActionFsm_CopyParamsFromSource(
            actionFsm, nullptr, reinterpret_cast<int>(paramsBytes));
    }

    character = CSF_AT(void*, self, 0x14);
    void* actionInterface = static_cast<uint8_t*>(character) + 0x04;
    return reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, actionInterface, 0)[0x58 / 4])(
            actionInterface, nullptr, paramsBytes);
}

HOOK(0x0047CAB0, AiAttackTasks_SubmitAttackParams, "auto");
