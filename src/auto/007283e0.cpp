#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_GetField0144, 0x00730100,
         (void* self, void* edx));

static uint8_t __fastcall C_008656d0_UpdateLinkedObjectFromControl(
    C_008656d0* self, void* /*edx*/)
{
    void* control =
        ((void* (__fastcall*)(void*, void*))self->vtbl[6])(self, nullptr);
    if (!control)
        return 0;

    if (!self->field_0030)
        return 0;

    const int controlType =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, control, 0)[24])(control, nullptr);

    uint8_t ready;
    if (controlType == 6)
    {
        ready = static_cast<uint8_t>(
            ((int (__fastcall*)(void*, void*))
                CSF_AT(void**, control, 0)[166])(control, nullptr));
    }
    else if (controlType == 11)
    {
        ready = static_cast<uint8_t>(
            CGuiSlider_GetField0144(control, nullptr) == 0);
    }
    else
    {
        return static_cast<uint8_t>(controlType);
    }

    if (!ready)
        return ready;

    uint8_t scratch[8];

    void* linkedObject = reinterpret_cast<void*>(self->field_0030);
    void** controlVtable = CSF_AT(void**, control, 0);
    void** linkedVtable = CSF_AT(void**, linkedObject, 0);

    const uint32_t firstValue =
        ((uint32_t (__fastcall*)(void*, void*, uint8_t*))
            controlVtable[88])(control, nullptr, scratch);

    linkedObject = reinterpret_cast<void*>(self->field_0030);
    ((void (__fastcall*)(void*, void*, uint32_t))
        linkedVtable[7])(linkedObject, nullptr, firstValue);

    linkedObject = reinterpret_cast<void*>(self->field_0030);
    linkedVtable = CSF_AT(void**, linkedObject, 0);
    controlVtable = CSF_AT(void**, control, 0);

    const uint32_t secondValue =
        ((uint32_t (__fastcall*)(void*, void*))
            controlVtable[41])(control, nullptr);

    linkedObject = reinterpret_cast<void*>(self->field_0030);
    ((void (__fastcall*)(void*, void*, uint32_t))
        linkedVtable[9])(linkedObject, nullptr, secondValue);

    linkedObject = reinterpret_cast<void*>(self->field_0030);
    linkedVtable = CSF_AT(void**, linkedObject, 0);
    return ((uint8_t (__fastcall*)(void*, void*))
        linkedVtable[2])(linkedObject, nullptr);
}

HOOK(0x007283E0, C_008656d0_UpdateLinkedObjectFromControl, "auto");
