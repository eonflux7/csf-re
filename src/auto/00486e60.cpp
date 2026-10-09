#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ActionTarget_ResolveFromSlot, 0x00485960, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));

static int __fastcall CCharActionFsm_ResolveActionTarget(
    CCharActionFsm* self, void* /*edx*/)
{
    if (!self->field_009c)
    {
        const int state = static_cast<int>(self->field_0010);
        if (state >= 17 && (state <= 18 || state == 21))
        {
            const uint32_t slot = self->field_0090;
            if (slot != UINT32_MAX)
                return reinterpret_cast<int*>(self->field_0078)[slot];
            return 0;
        }

        const int fallbackTarget = self->vSlotD8();
        auto* character = reinterpret_cast<C_00833ee8*>(self->field_0008);
        if (character->field_0228 && !character->field_0229)
        {
            if (C_004950e0_GetField398(character, nullptr))
            {
                const int slot = self->vSlotBC(32);
                if (slot != -1)
                    return self->vSlotC8(slot);
            }
        }
        return fallbackTarget;
    }

    void* target = reinterpret_cast<void*>(self->field_009c);
    const int targetType =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, target, 0)[0x4C / 4])(target, nullptr);

    if (targetType == 11)
        return self->vSlotD8();

    if (targetType != 12)
        return static_cast<int>(self->field_009c);

    const int state = static_cast<int>(self->field_0010);
    if (state < 13 || state > 14)
        return self->vSlotD8();

    return ActionTarget_ResolveFromSlot(self, nullptr);
}

HOOK(0x00486E60, CCharActionFsm_ResolveActionTarget, "auto");
