#include <cstdint>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, C_008657e0_DrawOutline, 0x00728DE0,
         (C_00865aa8* self, void* edx));

static int __fastcall C_00865aa8_UpdateLinkedValue(
    C_00865aa8* self, void* /*edx*/)
{
    void* linkedObject =
        ((void* (__fastcall*)(void*, void*))self->vtbl[6])(
            self, nullptr);

    if (linkedObject == nullptr)
        return 0;

    if (self->field_0064 != 0) {
        const int status =
            ((int (__fastcall*)(void*, void*))
                CSF_AT(void**, linkedObject, 0)[47])(
                    linkedObject, nullptr);

        if (static_cast<std::uint8_t>(status) == 0)
            return status;
    }

    void* valueObject = reinterpret_cast<void*>(self->field_0030);
    if (valueObject != nullptr) {
        const volatile std::uint32_t* currentValue =
            ((const std::uint32_t* (__fastcall*)(void*, void*))
                CSF_AT(void**, valueObject, 0)[12])(
                    valueObject, nullptr);

        std::uint32_t updatedValue[4];
        updatedValue[0] = currentValue[0];
        updatedValue[1] = currentValue[1];
        updatedValue[2] = currentValue[2];
        updatedValue[3] = currentValue[3];

        const std::uint8_t status =
            ((std::uint8_t (__fastcall*)(void*, void*))
                CSF_AT(void**, linkedObject, 0)[47])(
                    linkedObject, nullptr);

        if (status != 0)
            std::memcpy(&updatedValue[3], &self->field_0058, 4);
        else
            std::memcpy(&updatedValue[3], &self->field_0060, 4);

        ((void (__fastcall*)(void*, void*, const std::uint32_t*))
            CSF_AT(void**, valueObject, 0)[11])(
                valueObject, nullptr, updatedValue);
    }

    return C_008657e0_DrawOutline(self, nullptr);
}

HOOK(0x007293F0, C_00865aa8_UpdateLinkedValue, "auto");
