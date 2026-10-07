#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CGuiControl* __fastcall CGuiPanel_GetLastEligibleChildOrSelf(
    CGuiPanel* self, void* /*edx*/)
{
    const std::uint32_t initialBegin = self->field_0138;
    std::int32_t count = 0;
    if (initialBegin != 0) {
        count = static_cast<std::int32_t>(
            self->field_013c - initialBegin) >> 2;
    }

    for (std::int32_t index = count - 1; index >= 0; --index) {
        CGuiControl* child =
            reinterpret_cast<CGuiControl**>(self->field_0138)[index];

        const auto isEligible =
            reinterpret_cast<std::uint8_t (__fastcall*)(CGuiControl*, void*)>(
                CSF_AT(void**, child, 0)[0x164 / 4]);

        if (isEligible(child, nullptr) != 0) {
            return reinterpret_cast<CGuiControl**>(self->field_0138)[index];
        }
    }

    return self;
}

HOOK(0x0072B9E0, CGuiPanel_GetLastEligibleChildOrSelf, "auto");
