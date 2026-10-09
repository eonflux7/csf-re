#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Scene_SetInteractionHighlighting, 0x00452320,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateHeading, 0x00482DC0,
         (void* self, void* edx, int a2, float a3, char a4));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, CharActionFsm_SetGlobalActionMode, 0x004FC3B0,
         (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint8_t __fastcall CCharActionFsm_CompleteItemAction(
    CCharActionFsm* self, void* /*edx*/, int /*unused*/)
{
    const std::uint8_t enabled = self->field_0069;
    if (!enabled)
        return enabled;

    void* item = reinterpret_cast<void*>(self->field_009c);
    int itemType = 0;
    if (item)
    {
        itemType =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, item, 0)[0x4C / 4])(item, nullptr);
    }

    switch (itemType)
    {
    case 12:
    {
        CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(self->field_0008), 0x1F0) |= 2u;

        item = reinterpret_cast<void*>(self->field_009c);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, item, 0)[0x3C / 4])(item, nullptr);

        self->field_009c = 0;
        return static_cast<std::uint8_t>(self->vSlot134(2, 0));
    }

    case 13:
    {
        Scene_SetInteractionHighlighting(g_pCurrentScene, nullptr, 1);

        void* value = reinterpret_cast<void*>(Handle_GetValuePointer(
            reinterpret_cast<void*>(self->field_009c), nullptr));
        const int heading = CSF_AT(int, value, 0x1BC);

        value = reinterpret_cast<void*>(Handle_GetValuePointer(
            reinterpret_cast<void*>(self->field_009c), nullptr));
        const float turnRate = CSF_AT(float, value, 0x1C0);

        CCharActionFsm_UpdateHeading(self, nullptr, heading, turnRate, 1);

        item = reinterpret_cast<void*>(self->field_009c);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, item, 0)[0x3C / 4])(item, nullptr);

        // This tail forwarder passes the caller's zero to its virtual target.
        using ForwardItemAction = int (__fastcall*)(void*, void*, int);
        reinterpret_cast<ForwardItemAction>(
            C_00495140_ForwardInnerObjectVirtualCall)(
                reinterpret_cast<void*>(self->field_0008), nullptr, 0);

        void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        CharActionFsm_SetGlobalActionMode(manager, nullptr, 1);

        return static_cast<std::uint8_t>(self->vSlot134(0, 0));
    }

    case 14:
        return static_cast<std::uint8_t>(self->vSlot138(19));

    default:
        return static_cast<std::uint8_t>(self->vSlot134(0, 0));
    }
}

HOOK(0x00484B20, CCharActionFsm_CompleteItemAction, "auto");
