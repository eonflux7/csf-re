#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ReleaseSlotObject, 0x0042F610, (int* a1));

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

static void __fastcall S_004337e0_ReleaseEntrySlots(
    S_004337e0* self, void* /*edx*/, unsigned int index)
{
    if (index == static_cast<unsigned int>(-1))
        return;

    const uint32_t tableBegin = self->field_015c;
    if (tableBegin == 0)
        return;

    const int32_t count =
        static_cast<int32_t>(self->field_0160 - tableBegin) / 48;
    if (index >= static_cast<unsigned int>(count))
        return;

    auto* entry = reinterpret_cast<uint8_t*>(self->field_015c)
                + 48u * index;
    int* object = CSF_AT(int*, entry, 0x1C);
    if (object == nullptr)
        return;

    auto* slot = reinterpret_cast<uint8_t*>(self) + 32u * index + 0x168;
    for (int remaining = 8; remaining != 0; --remaining, slot += 4) {
        int* slotObject = CSF_AT(int*, slot, 0);
        if (slotObject != nullptr)
            ReleaseSlotObject(slotObject);
    }

    Hud_RenderWidgetAtScaledRect(object, nullptr);
}

HOOK(0x004337E0, S_004337e0_ReleaseEntrySlots, "auto");
