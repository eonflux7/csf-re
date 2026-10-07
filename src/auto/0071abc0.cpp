#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00864508_RemoveObjectAtIndex(
    C_00864508* self, void* /*edx*/, int index)
{
    const std::uint32_t byteOffset = static_cast<std::uint32_t>(index) << 2;

    void* entry = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0048), byteOffset);
    void* embeddedObject = reinterpret_cast<std::uint8_t*>(entry) + 4;
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, embeddedObject, 0)[19])(embeddedObject, nullptr);

    entry = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0048), byteOffset);
    if (((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[8])(entry, nullptr))
    {
        entry = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0048), byteOffset);
        void* associatedObject =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, entry, 0)[8])(entry, nullptr);

        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, associatedObject, 0)[22])(
                associatedObject, nullptr);
    }

    entry = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0048), byteOffset);
    ((void(__fastcall*)(C_00864508*, void*, void*, int))
        self->vtbl[31])(self, nullptr, entry, 0);

    entry = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0048), byteOffset);
    void* associatedObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[8])(entry, nullptr);

    int result = 0;
    if (associatedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, associatedObject, 0)[78])(
                associatedObject, nullptr, 14);

        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, associatedObject, 0)[61])(
                associatedObject, nullptr);

        void* objectManager = reinterpret_cast<void*>(self->field_0040);
        result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, objectManager, 0)[64])(
                objectManager, nullptr, associatedObject);
    }

    CSF_AT(void*, reinterpret_cast<void*>(self->field_0048), byteOffset) =
        nullptr;
    return result;
}

HOOK(0x0071ABC0, C_00864508_RemoveObjectAtIndex, "auto");
