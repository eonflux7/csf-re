#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuVideoAdvanced_ApplyChangedOption(
    CMenuVideoAdvanced* self, void* /*edx*/, void* event)
{
    if (!self->field_0024)
        return 0;
    if (!self->field_0050)
        return 0;

    std::uint32_t control = self->field_0028;
    std::size_t setterSlot;

    if (control && control == CSF_AT(std::uint32_t, event, 0x04))
    {
        setterSlot = 0x3C / 4;
    }
    else
    {
        control = self->field_002c;
        if (control && control == CSF_AT(std::uint32_t, event, 0x04))
        {
            setterSlot = 0x44 / 4;
        }
        else
        {
            control = self->field_0030;
            if (control && control == CSF_AT(std::uint32_t, event, 0x04))
            {
                setterSlot = 0x4C / 4;
            }
            else
            {
                control = self->field_0034;
                if (control && control == CSF_AT(std::uint32_t, event, 0x04))
                {
                    setterSlot = 0x54 / 4;
                }
                else
                {
                    control = self->field_0038;
                    if (control && control == CSF_AT(std::uint32_t, event, 0x04))
                    {
                        setterSlot = 0x5C / 4;
                    }
                    else
                    {
                        control = self->field_003c;
                        if (control && control == CSF_AT(std::uint32_t, event, 0x04))
                        {
                            setterSlot = 0x64 / 4;
                        }
                        else
                        {
                            control = self->field_0040;
                            if (control && control == CSF_AT(std::uint32_t, event, 0x04))
                            {
                                setterSlot = 0x6C / 4;
                            }
                            else
                            {
                                control = self->field_0044;
                                if (!control ||
                                    control != CSF_AT(std::uint32_t, event, 0x04))
                                {
                                    return static_cast<int>(control);
                                }
                                setterSlot = 0x74 / 4;
                            }
                        }
                    }
                }
            }
        }
    }

    void* changedControl = reinterpret_cast<void*>(control);
    int value =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, changedControl, 0)[0x2AC / 4])(
                changedControl, nullptr);

    // The getter can change the settings-interface pointer.
    void* settings = reinterpret_cast<void*>(self->field_0024);
    void** settingsVtable = CSF_AT(void**, settings, 0);

    if (setterSlot == 0x3C / 4)
    {
        // SETE changes only AL, retaining the getter's upper 24 bits.
        const std::uint32_t bits = static_cast<std::uint32_t>(value);
        value = static_cast<int>((bits & 0xFFFFFF00u) | (value == 0 ? 1u : 0u));
    }

    return ((int (__fastcall*)(void*, void*, int))
        settingsVtable[setterSlot])(settings, nullptr, value);
}

HOOK(0x005491A0, CMenuVideoAdvanced_ApplyChangedOption, "auto");
