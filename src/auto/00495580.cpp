#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, CharActionFsm_CanStartAction, 0x00483050, (void* self, void* , char a2));
CSF_FUNC(char, __fastcall, CCharActionFsm_CanUseItem, 0x00487400, (CCharActionFsm* self, void* , uint32_t* item));
CSF_FUNC(int, __fastcall, ToggleFlagMask, 0x00493AE0, (void* self, void* , int mask));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static char __fastcall C_00833b88_RequestSecondaryAction(
    C_00833b88* self, void* /*edx*/)
{
    const uint32_t actionFlags = CSF_AT(uint32_t, self, 0x2E0);
    if ((actionFlags & 0x12) != 0 || (actionFlags & 0x4000) != 0)
        return static_cast<char>(actionFlags);

    void* actionFsm = reinterpret_cast<void*>(self->field_0044);
    C_0086754c* currentItem =
        ((C_0086754c* (__fastcall*)(void*, void*))
             CSF_AT(void**, actionFsm, 0)[0xD8 / 4])(actionFsm, nullptr);

    const uint32_t secondaryFlags = CSF_AT(uint32_t, self, 0x2E4);
    if ((secondaryFlags & 0x40000) != 0)
    {
        const uint32_t updatedFlags = secondaryFlags & 0xFFFBFFFF;
        CSF_AT(uint32_t, self, 0x2E4) = updatedFlags;
        return static_cast<char>(updatedFlags);
    }

    if ((secondaryFlags & 0x40) != 0 ||
        CharActionFsm_CanStartAction(actionFsm, nullptr, 0))
    {
        const uint8_t currentFlags = CSF_AT(uint8_t, self, 0x2E0);
        if ((currentFlags & 0x80) == 0)
            return static_cast<char>(ToggleFlagMask(self, nullptr, 64));

        if (!currentItem)
            return static_cast<char>(currentFlags);

        const int itemCount = GetCurrentItemCount(currentItem, nullptr);
        if (itemCount > 0)
        {
            CSF_AT(uint32_t, self, 0x2E4) =
                (CSF_AT(uint32_t, self, 0x2E4) & 0xFFFFFF3F) | 0x40;
        }
        return static_cast<char>(itemCount);
    }

    if (!CharActionFsm_CanStartAction(actionFsm, nullptr, 1))
        return 0;

    const char canUseItem = CCharActionFsm_CanUseItem(
        reinterpret_cast<CCharActionFsm*>(actionFsm), nullptr,
        CSF_AT(uint32_t*, actionFsm, 0x8C));
    if (!canUseItem)
        return canUseItem;

    const uint32_t updatedFlags =
        CSF_AT(uint32_t, self, 0x2E4) | 0x40000;
    CSF_AT(uint32_t, self, 0x2E4) = updatedFlags;
    return static_cast<char>(updatedFlags);
}

HOOK(0x00495580, C_00833b88_RequestSecondaryAction, "auto");
