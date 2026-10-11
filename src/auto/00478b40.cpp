#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, C_008317c8_PrepareEntryAction, 0x00476220, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ActionFsm_ResetSubObjectTarget, 0x004783A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008317c8_UpdateEntryState, 0x00478420, (void* self, void* edx));

static int __fastcall C_008317c8_ProcessEntryAction(
    C_008317c8* self, void* /*edx*/, int action)
{
    C_008317c8_PrepareEntryAction(self, nullptr, action);

    int result = self->vResolveEntryLookupValue();
    if (!result)
        return result;

    void* entry = reinterpret_cast<void*>(result);
    int entryIndex =
        ((int(__fastcall*)(void*, void*, int))
            self->vtbl[0xF4 / 4])(self, nullptr, action);

    if (entryIndex != -1 &&
        !((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0xA8 / 4])(entry, nullptr))
    {
        void* owner = nullptr;
        if (C_008319c0_GetSubObjectField74(
                reinterpret_cast<C_008319c0*>(self), nullptr) != 19 ||
            CSF_AT(uint8_t,
                owner = reinterpret_cast<void*>(self->field_0004),
                0x228) != 0)
        {
            owner = reinterpret_cast<void*>(self->field_0004);
            void* controller =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

            ((void(__fastcall*)(void*, void*, void*, int, int))
                CSF_AT(void**, controller, 0)[0xA0 / 4])(
                    controller, nullptr, entry, entryIndex, 0);
        }
        else
        {
            void* controller =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

            ((void(__fastcall*)(void*, void*, void*, int, int))
                CSF_AT(void**, controller, 0)[0xAC / 4])(
                    controller, nullptr, entry, entryIndex, 2200);
        }
    }

    C_008317c8_UpdateEntryState(self, nullptr);
    result = static_cast<int32_t>(self->field_006c);

    if (result < 0 &&
        (C_008319c0_GetSubObjectField74(
             reinterpret_cast<C_008319c0*>(self), nullptr) == 8 ||
         (result = static_cast<int32_t>(self->field_0070)) != 0))
    {
        self->vSlotDC(0);
        return ActionFsm_ResetSubObjectTarget(self, nullptr);
    }

    return result;
}

HOOK(0x00478B40, C_008317c8_ProcessEntryAction, "auto");
