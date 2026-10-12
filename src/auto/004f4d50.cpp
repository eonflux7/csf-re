#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_0083f160_SerializeObjectReferences(
    C_0083f160* self, void* /*edx*/, bool* archive)
{
    bool skip = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        skip = !((bool(__fastcall*)(void*, void*, int, bool*))
                     CSF_AT(void**, serializer, 0)[0x80 / 4])(
            serializer, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = skip;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!skip)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0004, archive);
    }

    bool active = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0008, archive);
    }

    active = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&CSF_AT(uint32_t, self, 0x0C), archive);
    }

    active = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (active)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, bool*))
                 CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004F4D50, C_0083f160_SerializeObjectReferences, "auto");
