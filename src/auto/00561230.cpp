#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0,
    (void* reference, void* edx));

static uint32_t* __fastcall CMenuMpSessions_UpdateControlPosition(
    CMenuMpSessions* self, void* /*edx*/)
{
    void* reference = reinterpret_cast<uint8_t*>(self) + 4;
    void* object = GetReferencedObject(reference, nullptr);

    uint32_t* result = ((uint32_t* (__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);

    if (!result)
        return result;

    void* verticalObject = GetReferencedObject(reference, nullptr);
    void* horizontalObject = GetReferencedObject(reference, nullptr);

    void* verticalBounds = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, verticalObject, 0)[0x30 / 4])(
            verticalObject, nullptr);
    const uint32_t bottom = CSF_AT(uint32_t, verticalBounds, 0x18);

    void* horizontalBounds = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, horizontalObject, 0)[0x30 / 4])(
            horizontalObject, nullptr);

    void* control = reinterpret_cast<void*>(self->field_0040);
    const uint32_t right = CSF_AT(uint32_t, horizontalBounds, 0x14);

    uint32_t sizeBuffer[2];
    uint32_t* size = ((uint32_t* (__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, control, 0)[0x160 / 4])(
            control, nullptr, sizeBuffer);

    const uint32_t width = size[0];
    const uint32_t height = size[1];
    const uint32_t left = right - width;
    control = reinterpret_cast<void*>(self->field_0040);
    const uint32_t top = bottom - height;
    uint32_t position[2] = {left, top};

    result = ((uint32_t* (__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x148 / 4])(control, nullptr);

    if (position[0] != result[0] || position[1] != result[1])
    {
        control = reinterpret_cast<void*>(self->field_0040);
        return ((uint32_t* (__fastcall*)(void*, void*, uint32_t*, int))
            CSF_AT(void**, control, 0)[0x14c / 4])(
                control, nullptr, position, 1);
    }

    return result;
}

HOOK(0x00561230, CMenuMpSessions_UpdateControlPosition, "auto");
