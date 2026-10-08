#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0,
         (void* array, void* edx, int growth));

static int __fastcall CMenuMessageBox_RegisterMaskedButton(
    CMenuMessageBox* self, void* /*edx*/, void* button, uint32_t mask)
{
    // The original leaves EAX unspecified when button is null.
    if (!button)
        return 0;

    int flag = 1;
    if ((self->field_007c & mask) != 0)
    {
        void* array = reinterpret_cast<uint8_t*>(self) + 0x954;
        if (CSF_AT(uint32_t, self, 0x95C) >= self->field_0958)
            GrowButtonArray(array, nullptr, CSF_AT(int, self, 0x960));

        const uint32_t count = CSF_AT(uint32_t, self, 0x95C);
        auto* slot = reinterpret_cast<uint32_t*>(
            self->field_0954 + count * 4u);
        CSF_AT(uint32_t, self, 0x95C) = count + 1u;
        *slot = reinterpret_cast<uint32_t>(button);
        flag = 0;
    }

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, button, 0)[0xA8 / 4])(button, nullptr, flag);
}

HOOK(0x0052AEE0, CMenuMessageBox_RegisterMaskedButton, "auto");
