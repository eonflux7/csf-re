#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_6A7860, 0x006A7860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6A7B50, 0x006A7B50, (void* self, void* edx, int a2, int* a3));

static char __fastcall S_006a7640_ProcessGatheringResult(
    S_006a7640* self, void* /*edx*/, int /*unused*/)
{
    auto* result = reinterpret_cast<C_0085e760*>(self->field_0090);
    const uint32_t state = CSF_AT(uint32_t, result, 0x0C);

    if (state == 1)
        return 0;

    if (result)
    {
        result->vSlot0(1);
        self->field_0090 = 0;
    }

    switch (state)
    {
    case 2:
        return sub_6A7860(self, nullptr);

    case 3:
    {
        // Preserve the original read after releasing and clearing the result.
        auto* releasedResult =
            reinterpret_cast<uint8_t*>(self->field_0090);
        alignas(8) uint8_t statusBytes[0xC];
        int* status = reinterpret_cast<int*>(statusBytes);
        status[0] = CSF_AT(int, releasedResult, 0x44);
        status[1] = CSF_AT(int, releasedResult, 0x48);
        status[2] = CSF_AT(int, releasedResult, 0x4C);

        sub_6A7B50(self, nullptr, 0, status);
        return sub_6A7860(self, nullptr);
    }

    default:
        self->field_02b0 = 1229730866;
        return sub_6A7860(self, nullptr);
    }
}

HOOK(0x006A8880, S_006a7640_ProcessGatheringResult, "auto");
