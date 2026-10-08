#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static const auto CGuiControl_CheckInteractionState =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(CSF_ADDR_FN(0x00718700));

static uint32_t __fastcall CGuiEdit_ResetEditState(
    CGuiEdit* self, void* /*edx*/, int /*arg2*/, int /*arg3*/)
{
    uint32_t result =
        reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    result = static_cast<uint32_t>(
        CGuiControl_CheckInteractionState(self, nullptr));
    if (static_cast<uint8_t>(result) == 0)
        return result;

    if (self->field_0148 == 0)
    {
        result =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);
        if (static_cast<uint8_t>(result) == 0)
            return result;
    }

    result = self->field_01b4;
    const int32_t state = static_cast<int32_t>(result);
    if (state < 0 || state > 1)
        return result;

    result = UINT32_MAX;
    self->field_01b4 = result;
    self->field_01bc = result;
    self->field_01c0 = 0x10;
    self->field_01c4 = 0;
    return result;
}

HOOK(0x00721200, CGuiEdit_ResetEditState, "auto");
