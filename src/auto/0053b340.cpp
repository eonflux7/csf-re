#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __cdecl, GameOptions_NotifyUnusedOptionState, 0x00506AC0,
         (std::uint32_t state));

static void __fastcall CMenuGameOptions_HandleOptionButtonEvent(
    CMenuGameOptions* self, void* /*edx*/, void* event)
{
    if (self->field_0024 == 0)
        return;

    std::uint32_t button = self->field_0030;
    std::size_t setterSlot;
    std::uint32_t comparisonValue = 0;

    if (button != 0 && button == CSF_AT(std::uint32_t, event, 0x04))
    {
        setterSlot = 0x1C / 4;
    }
    else if ((button = self->field_0034) != 0 &&
             button == CSF_AT(std::uint32_t, event, 0x04))
    {
        setterSlot = 0x24 / 4;
    }
    else if ((button = self->field_0038) != 0 &&
             button == CSF_AT(std::uint32_t, event, 0x04))
    {
        setterSlot = 0x2C / 4;
    }
    else if ((button = self->field_003c) != 0 &&
             button == CSF_AT(std::uint32_t, event, 0x04))
    {
        setterSlot = 0x64 / 4;
        comparisonValue = 1;
    }
    else if ((button = self->field_0040) != 0 &&
             button == CSF_AT(std::uint32_t, event, 0x04))
    {
        void* control = reinterpret_cast<void*>(button);
        const std::uint32_t state =
            reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x2AC / 4])(control, nullptr);
        GameOptions_NotifyUnusedOptionState(state);
        return;
    }
    else if ((button = self->field_0044) != 0 &&
             button == CSF_AT(std::uint32_t, event, 0x04))
    {
        setterSlot = 0x70 / 4;
    }
    else
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, self, 0)[0x68 / 4])(self, nullptr, event);
        return;
    }

    void* control = reinterpret_cast<void*>(button);
    void* options = reinterpret_cast<void*>(self->field_0024);
    void** controlVtable;
    void** optionsVtable;

    // The final button loads the two vtables in the opposite order.
    if (setterSlot == 0x70 / 4)
    {
        optionsVtable = CSF_AT(void**, options, 0);
        controlVtable = CSF_AT(void**, control, 0);
    }
    else
    {
        controlVtable = CSF_AT(void**, control, 0);
        optionsVtable = CSF_AT(void**, options, 0);
    }

    const std::uint32_t state =
        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
            controlVtable[0x2AC / 4])(control, nullptr);

    options = reinterpret_cast<void*>(self->field_0024);

    // NEG changes all of EAX; SBB AL,AL / INC AL replaces only its low byte.
    const std::uint32_t negated = 0u - (state - comparisonValue);
    const std::uint32_t argument =
        (negated & 0xFFFFFF00u) | (state == comparisonValue ? 1u : 0u);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
        optionsVtable[setterSlot])(options, nullptr, argument);
}

HOOK(0x0053B340, CMenuGameOptions_HandleOptionButtonEvent, "auto");
