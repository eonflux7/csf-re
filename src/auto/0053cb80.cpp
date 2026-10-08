#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuSoundOptions_ApplyToggleSelection(
    CMenuSoundOptions* self, void* /*edx*/)
{
    std::uint32_t objectAddress = self->field_002c;
    if (objectAddress == 0)
        return 0;

    objectAddress = self->field_0024;
    if (objectAddress == 0)
        return 0;

    void* selector = reinterpret_cast<void*>(objectAddress);
    const int activeResult = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, selector, 0)[0x6C / 4])(selector, nullptr);
    if (static_cast<std::uint8_t>(activeResult) == 0)
        return activeResult;

    selector = reinterpret_cast<void*>(self->field_0024);
    void* settings = reinterpret_cast<void*>(self->field_002c);
    void** selectorVtable = CSF_AT(void**, selector, 0);
    void** settingsVtable = CSF_AT(void**, settings, 0);

    const int selection = ((int(__fastcall*)(void*, void*, int))
        selectorVtable[0x2AC / 4])(selector, nullptr, 0);

    // NEG EAX; SBB AL, AL; INC AL changes only the low byte
    // after negation, retaining the upper 24 bits.
    const std::uint32_t negated =
        0u - static_cast<std::uint32_t>(selection);
    const std::uint32_t argument =
        (negated & 0xFFFFFF00u) | (selection == 0 ? 1u : 0u);

    settings = reinterpret_cast<void*>(self->field_002c);
    return ((int(__fastcall*)(void*, void*, int))
        settingsVtable[0x34 / 4])(
            settings, nullptr, static_cast<int>(argument));
}

HOOK(0x0053CB80, CMenuSoundOptions_ApplyToggleSelection, "auto");
