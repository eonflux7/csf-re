#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AutoTask_CopyOrder, 0x00489BB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AutoTask_StartPrimaryOrder, 0x00489F50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AutoTask_StartSecondaryOrder, 0x0048A030, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_00833708_SendOrder(
    C_00833708* self, void* /*edx*/, int taskIndex, int orderAddress)
{
    const uint32_t taskOffset = 96 * static_cast<uint32_t>(taskIndex);
    auto* entry = reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset;
    const uint32_t state = CSF_AT(uint32_t, entry, 4);

    if (state == 0)
        return reinterpret_cast<uint32_t*>(entry);

    auto* order = reinterpret_cast<uint8_t*>(orderAddress);
    const uint32_t orderType = CSF_AT(uint32_t, order, 0);

    if (orderType == 0 || orderType == 1)
    {
        if (state != 1)
            return reinterpret_cast<uint32_t*>(entry);

        auto* owner = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(uint32_t, entry, 48) = CSF_AT(uint32_t, owner, 32);
        CSF_AT(uint32_t, entry, 52) = CSF_AT(uint32_t, owner, 36);

        CSF_AT(uint32_t,
            reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset, 56) =
            CSF_AT(uint32_t, order, 4);

        auto* currentEntry =
            reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset;
        CSF_AT(uint32_t, currentEntry, 64) = CSF_AT(uint32_t, order, 12);
        CSF_AT(uint32_t, currentEntry, 68) = CSF_AT(uint32_t, order, 16);

        currentEntry = reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset;
        CSF_AT(uint32_t, currentEntry, 72) = CSF_AT(uint32_t, order, 20);
        CSF_AT(uint32_t, currentEntry, 76) = CSF_AT(uint32_t, order, 24);
        CSF_AT(uint32_t, currentEntry, 80) = CSF_AT(uint32_t, order, 28);

        CSF_AT(uint8_t,
            reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset, 88) =
            CSF_AT(uint8_t, order, 32);
        CSF_AT(uint8_t,
            reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset, 91) =
            CSF_AT(uint8_t, order, 33);

        if (orderType == 0)
        {
            CSF_AT(uint32_t,
                reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset, 4) = 2;
            AutoTask_StartPrimaryOrder(self, nullptr, taskIndex);
        }
        else
        {
            CSF_AT(uint32_t,
                reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset, 4) = 3;
            AutoTask_StartSecondaryOrder(self, nullptr, taskIndex);
        }
    }
    else if (orderType == 2 && state != 1)
    {
        CSF_AT(uint32_t, entry, 4) = 4;
    }

    return AutoTask_CopyOrder(
        reinterpret_cast<uint8_t*>(self->field_000c) + taskOffset + 8,
        nullptr, orderAddress);
}

HOOK(0x0048A4D0, C_00833708_SendOrder, "auto");
